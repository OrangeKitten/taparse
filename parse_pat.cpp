#include "parse_pat.h"
#include "log.h"
#include <stdlib.h>
#include "parse_type.h"
void ParsePAT::Decode(uint8_t *data, int length)
{
    int offset = 0;
    Pat_info_.table_id = data[0];
    Pat_info_.section_syntax_indicator = (data[1] & 0x80) >> 7;
    if (Pat_info_.section_syntax_indicator != 1)
    {
        return;
    }
    Pat_info_.section_length = ((data[1] & 0x0f) << 8 | data[2]);
    Pat_info_.transport_stream_id = data[3] << 8 | data[4];
    Pat_info_.version_number = (data[5] & 0x3e) >> 1;
    Pat_info_.current_next_indicator = (data[5] & 0x01);
    Pat_info_.section_number = data[6];
    Pat_info_.last_section_number = data[7];
    offset += 8;
    data = data + offset;
    int pmt_info_length = Pat_info_.section_length - PATHEADNUM - CRCNUM+3;
    // for(int i = 0;i<pmt_info_length+4;i++) {
    //     log_info("%0x ",data[i]);
        
    // }exit(0);
    for (int index = 0; index < pmt_info_length; index++)
    {
        PmtInfo pmtinfo;
        pmtinfo.program_number = (data[index] << 8) | data[++index];
        if (pmtinfo.program_number != 0)
        {
            pmtinfo.pmt_pid = (data[++index] & 0x1f) << 8 | data[++index];
        }
        else
        {
            pmtinfo.network_pid = (data[++index] & 0x1f) << 8 | data[++index];
        }
        Pat_info_.pmt_info.push_back(pmtinfo);
    }
    Pat_info_.crc32 = (data[pmt_info_length]<<24)|(data[++pmt_info_length]<<16)|(data[++pmt_info_length]<<8)|(data[++pmt_info_length]);
    Dump();
}
void ParsePAT::Dump()const
{
    log_info("table_id = %0x", Pat_info_.table_id);
    log_info("section_syntax_indicator = %0x", Pat_info_.section_syntax_indicator);
    log_info("section_length = %0x", Pat_info_.section_length);
    log_info("transport_stream_id = %0x", Pat_info_.transport_stream_id);
    log_info("version_number = %0x", Pat_info_.version_number);
    log_info("current_next_indicator = %0x", Pat_info_.current_next_indicator);
    log_info("section_number = %0x", Pat_info_.section_number);
    log_info("last_section_number = %0x", Pat_info_.last_section_number);
    for (auto &pmt_info : Pat_info_.pmt_info)
    {
        log_info("program_number = %0x", pmt_info.program_number);
        if (pmt_info.program_number == 0)
        {
            log_info("network_pid = %0x", pmt_info.network_pid);
        }
        else
        {
            log_info("pmt_pid = %0x", pmt_info.pmt_pid);
        }
    }
       log_info("crc = %0x", Pat_info_.crc32);
}