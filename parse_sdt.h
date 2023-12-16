#ifndef __PARSESDT__
#define __PARSESDT__
#include <stdint.h>
#include <vector>
#include "parse_descriptor.h"

typedef struct {
    uint16_t service_id;
    uint8_t EIT_schedule_flag;
    uint8_t EIT_present_following_flag;
    uint8_t running_status;
    uint8_t free_CA_mode;
    uint16_t descriptor_loop_length;
     ParseDescriptor des_streams;  // ES 列表
}ServiceDescriptionLoop;

// 定义 SDT 结构体
typedef struct {
  uint8_t table_id; // 固定为 0x02 表示 PMT
  uint8_t section_syntax_indicator;
  uint16_t section_length;        
  uint16_t transport_stream_id;       
  uint8_t version_number;        
  uint8_t current_next_indicator; 
  uint8_t section_number;         
  uint8_t last_section_number; 
  uint16_t original_network_id;             
  std::vector<ServiceDescriptionLoop> service_des_info;  
  uint32_t crc32;
} SDT;

class ParseSDT{
    public:
     ParseSDT() = default;
  ~ParseSDT() = default;
  void Decode(uint8_t *data, int length);
  void Dump() const;
  void Debug(uint8_t *data, int length);
    public:
    SDT Sdt_info_;
};
#endif
