#include "parse_pmt.h"
#include "log.h"
#include "parse_type.h"
#include <stdlib.h>
void ParsePMT::Decode(uint8_t *data, int length) {
//   Debug(data, length);
  int offset = 0;
  Pmt_info_.table_id = data[0];
  Pmt_info_.section_syntax_indicator = (data[1] & 0x80) >> 7;
  if (Pmt_info_.section_syntax_indicator != 1) {
    return;
  }
  Pmt_info_.section_length = ((data[1] & 0x0f) << 8 | data[2]);
  Pmt_info_.program_number = data[3] << 8 | data[4];
  Pmt_info_.version_number = (data[5] & 0x3e) >> 1;
  Pmt_info_.current_next_indicator = (data[5] & 0x01);
  Pmt_info_.section_number = data[6];
  Pmt_info_.last_section_number = data[7];
  Pmt_info_.PCR_PID = ((data[8] & 0x1f) << 8) | data[9];
  Pmt_info_.program_info_length = ((data[10] & 0x0f) << 8) | data[11];
  offset += 12;
  //移动到pmthead的下一个字节
  data = data + offset;

  offset = 0;
  for (int descriptor_index = 0;
       descriptor_index < Pmt_info_.program_info_length; descriptor_index++) {
  }
  offset += Pmt_info_.program_info_length;
  int ES_length = Pmt_info_.section_length + 3 - Pmt_info_.program_info_length -
                  CRCNUM - PMTHEADNUM;
  for (int ES_index = 0; ES_index < ES_length; ES_index++) {
    ES_info es_info;
    es_info.stream_type = data[ES_index++];
    es_info.elementary_pid = (data[ES_index]&0x1f)<<8|data[++ES_index];
    es_info.ES_info_length = (data[++ES_index]&0x0f)<<8|data[++ES_index];
    es_info.des_streams.Parse_Descriptor(data+ES_index+1,es_info.ES_info_length);
    // for(int i = 1 ;i<=es_info.ES_info_length;i++) {
        //     ParseDescriptor des_info;
        //     des_info.descriptor_tag = data[ES_index+i];
        //     des_info.descriptor_length = data[ES_index+(i+1)];
        //     i+=des_info.descriptor_length+1;
        //     es_info.des_streams.push_back(des_info);

    // }
    ES_index+=es_info.ES_info_length;
    Pmt_info_.elementary_streams.push_back(es_info);

  }

  offset += ES_length;
  Pmt_info_.crc32 = (data[offset] << 24) | (data[++offset] << 16) |
                    (data[++offset] << 8) | (data[++offset]);
//   Dump();
//   exit(0);
}
void ParsePMT::Dump() const {
  log_info("table_id = %0x", Pmt_info_.table_id);
  log_info("section_syntax_indicator = %0x",
           Pmt_info_.section_syntax_indicator);
  log_info("section_length = %0x", Pmt_info_.section_length);
  log_info("transport_stream_id = %0x", Pmt_info_.program_number);
  log_info("version_number = %0x", Pmt_info_.version_number);
  log_info("current_next_indicator = %0x", Pmt_info_.current_next_indicator);
  log_info("section_number = %0x", Pmt_info_.section_number);
  log_info("last_section_number = %0x", Pmt_info_.last_section_number);
  log_info("PCR_PID = %0x", Pmt_info_.PCR_PID);
  log_info("program_info_length = %0x", Pmt_info_.program_info_length);
  for(auto &es_info:Pmt_info_.elementary_streams) {
     log_info("  stream_type = %0x", es_info.stream_type);
     log_info("  elementary_pid = %0x", es_info.elementary_pid);
     log_info("  ES_info_length = %0x", es_info.ES_info_length);
     //  for(auto&des_info:es_info.des_streams){
         //      log_info("      descriptor_tag = %0x", des_info.descriptor_tag);
           //        log_info("      descriptor_length = %0x", des_info.descriptor_length);
     //  }
  }
  log_info("crc = %0x", Pmt_info_.crc32);
}

void ParsePMT::Debug(uint8_t *data, int length) {
  for (int i = 0; i < length; i++) {
    log_debug("%0x  %p", data[i], &data[i]);
  }
}