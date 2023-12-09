#ifndef __PARSEPAT__
#define __PARSEPAT__
#include <stdint.h>
#include <vector>
// 定义 PMT 结构体
typedef struct  {
    uint16_t program_number;  // 节目号
    uint8_t reserved;
    uint16_t pmt_pid;         // PMT 的 PID
    uint16_t network_pid;         // PMT 的 PID
}PmtInfo;
// 定义 PAT 结构体
typedef struct  {
    uint8_t table_id;                   // 固定为 0x00 表示 PAT
    uint8_t section_syntax_indicator;   //比特字段设置为1
    uint16_t section_length;            // 表示 section 的长度（包含CRC）
    uint16_t transport_stream_id;       // 传输流 ID
    uint8_t version_number;             // 版本号
    uint8_t current_next_indicator;     // 表示 PAT 是当前有效还是下一个有效
    uint8_t section_number;             // 表示 section 的编号
    uint8_t last_section_number;        // 表示最后一个 section 的编号
    std::vector<PmtInfo> pmt_info;          // 节目列表
    uint32_t crc32;                     // CRC32 校验值
}Pat;

class ParsePAT{
    public:
    ParsePAT() = default;
    ~ParsePAT() = default;
    void Parse_PAT();
    void Decode(uint8_t*data,int length);
   void Dump( )const;
    public:
    Pat Pat_info_;
};
#endif