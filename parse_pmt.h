#ifndef __PARSEPMT__
#define __PARSEPMT__

#include <stdint.h>
#include <vector>
#include "parse_descriptor.h"


typedef struct  {
    uint8_t stream_type;   // 流类型
    uint16_t elementary_pid;  // ES 的 PID
    uint16_t ES_info_length;
    ParseDescriptor des_streams;  // ES 列表
}ES_info;
// 定义 PMT 结构体
typedef struct {
  uint8_t table_id; // 固定为 0x02 表示 PMT
  uint8_t section_syntax_indicator;
  uint16_t section_length;        // 表示 section 的长度（包含CRC）
  uint16_t program_number;        // 节目号
  uint8_t version_number;         // 版本号
  uint8_t current_next_indicator; // 表示 PMT 是当前有效还是下一个有效
  uint8_t section_number;         // 表示 section 的编号
  uint8_t last_section_number;  // 表示最后一个 section 的编号
  uint16_t PCR_PID;             // PCR（Program Clock Reference）的 PID
  uint16_t program_info_length; // 节目信息的长度
  std::vector<ES_info> elementary_streams;  // ES 列表
  uint32_t crc32; // CRC32 校验值
                  // 其他 PMT 相关信息...
} PMT;

class ParsePMT {
public:
  ParsePMT() = default;
  ~ParsePMT() = default;
  void Decode(uint8_t *data, int length);
  void Dump() const;
  void Debug(uint8_t *data, int length);
public:
  PMT Pmt_info_;
};
#endif
