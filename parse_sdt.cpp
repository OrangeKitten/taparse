#include "parse_sdt.h"
#include "log.h"
#include "parse_type.h"
#include <stdlib.h>
void ParseSDT::Decode(uint8_t *data, int length) {
   //Debug(data, length);
  int offset = 0;
  Sdt_info_.table_id = data[0];
  Sdt_info_.section_syntax_indicator = (data[1] & 0x80) >> 7;
  if (Sdt_info_.section_syntax_indicator != 1) {
    return;
  }
  Sdt_info_.section_length = ((data[1] & 0x0f) << 8 | data[2]);
  Sdt_info_.transport_stream_id = data[3] << 8 | data[4];
  Sdt_info_.version_number = (data[5] & 0x3e) >> 1;
  Sdt_info_.current_next_indicator = (data[5] & 0x01);
  Sdt_info_.section_number = data[6];
  Sdt_info_.last_section_number = data[7];
  Sdt_info_.original_network_id = ((data[8] & 0x1f) << 8) | data[9];
  offset += 11;
  //移动到sdthead的下一个字节
  data = data + offset;
  offset = 0;
  int servuce_des_length = Sdt_info_.section_length + 3  -
                  CRCNUM - SDTHEADNUM;
  for (int ES_index = 0; ES_index < servuce_des_length; ES_index++) {
    ServiceDescriptionLoop service_des_info;
    service_des_info.service_id = (data[ES_index++]<<8)|data[ES_index++];
    service_des_info.EIT_schedule_flag = (data[ES_index]&0x2)>>1;
    service_des_info.EIT_present_following_flag = (data[ES_index++]&0x1);
    service_des_info.running_status = (data[ES_index]&0xE0)>>5;
    service_des_info.free_CA_mode = (data[ES_index]&0x10)>>4;
    service_des_info.descriptor_loop_length = (data[ES_index]&0x0f)<<8|data[++ES_index];
    for(int i = 1 ;i<=service_des_info.descriptor_loop_length;i++) {
        ParseDescriptor des_info;
        des_info.descriptor_tag = data[ES_index+i];
        des_info.descriptor_length = data[ES_index+(i+1)];
        i+=des_info.descriptor_length+1;
        service_des_info.des_streams.push_back(des_info);
    }
    ES_index+=service_des_info.descriptor_loop_length;
    Sdt_info_.service_des_info.push_back(service_des_info);

  }
    

  offset += servuce_des_length;
  Sdt_info_.crc32 = (data[offset] << 24) | (data[++offset] << 16) |
                    (data[++offset] << 8) | (data[++offset]);
  Dump();
  
}
void ParseSDT::Dump() const {
  log_info("table_id = %0x", Sdt_info_.table_id);
  log_info("section_syntax_indicator = %0x",
           Sdt_info_.section_syntax_indicator);
  log_info("section_length = %0x", Sdt_info_.section_length);
  log_info("transport_stream_id = %0x", Sdt_info_.transport_stream_id);
  log_info("version_number = %0x", Sdt_info_.version_number);
  log_info("current_next_indicator = %0x", Sdt_info_.current_next_indicator);
  log_info("section_number = %0x", Sdt_info_.section_number);
  log_info("last_section_number = %0x", Sdt_info_.last_section_number);
  log_info("original_network_id = %0x", Sdt_info_.original_network_id);
  for(auto &service_des_info:Sdt_info_.service_des_info) {
     log_info("  service_id = %0x", service_des_info.service_id);
     log_info("  EIT_schedule_flag = %0x", service_des_info.EIT_schedule_flag);
     log_info("  EIT_present_following_flag = %0x", service_des_info.EIT_present_following_flag);
     log_info("  running_status = %0x", service_des_info.running_status);
     log_info("  free_CA_mode = %0x", service_des_info.free_CA_mode);
     log_info("  descriptor_loop_length = %0x", service_des_info.descriptor_loop_length);
     for(auto&des_info:service_des_info.des_streams){
         log_info("      descriptor_tag = %0x", des_info.descriptor_tag);
           log_info("      descriptor_length = %0x", des_info.descriptor_length);
     }
  }
  log_info("crc = %0x", Sdt_info_.crc32);
}

void ParseSDT::Debug(uint8_t *data, int length) {
  for (int i = 0; i < length; i++) {
    log_debug("%0x  %p", data[i], &data[i]);
  }
}