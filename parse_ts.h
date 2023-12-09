#ifndef __PARSETS__
#define __PARSETS__
#include <string>
#include <vector>
#include "parse_psi.h"
#include "log.h"
const int TS_PACKET_SIZE = 188;
const int TTS_PACKET_SIZE = 192;
const int TS_PACKET_FECTS_SIZE = 204;
typedef struct
{
    uint8_t sync_byte;
    uint8_t transport_error_indicator;
    uint8_t payload_unit_start_indicator;
    uint8_t transport_priority;
    uint16_t PID;
    uint8_t transport_scrambling_control;
    uint8_t adaptation_field_control;
    uint8_t continuity_counter;
    // Add more fields as needed
} TsPacketHeader;

typedef struct {
    bool is_entire_data;
    uint16_t PID;
    uint8_t TABLE_ID;
    uint16_t entire_section_length_;
    std::vector<uint8_t> section_buffer;
}section_data;

class Parse_Ts
{
    //    enum class  PacketType {
    //     PSI,
    //     PES
    //    };
public:
    Parse_Ts(std::string file_name) : file_name_(file_name), pos_(0), file_size_(0), exit_section_(false)
    {
    }
    ~Parse_Ts()
    {
        log_info("file_content_ = %p",file_content_);
        free (file_content_);
        file_content_ = nullptr;
        fclose(log_file_info_);
        fclose(log_file_debug_);
    }
    bool ParseInit();
    bool ParseTS();
    int get_file_size();

private:
    void GetDataFromFile();
    TsPacketHeader ts_head_;
    int ParseTsHead(uint8_t *PaketHead);
    void ParsePayload();
    int CheckHead(uint8_t*temp_file_content_,int offset);
    void Dump()const ;
    void Debug(uint8_t *data, int length);
    //void DispatchDeconde(uint8_t *section_data, int length);
    // PacketType get_packet_type(int pid);
    uint8_t check_TS_unit_size();

public:
    uint8_t *file_content_;
    FILE *ts_file_;
    FILE *log_file_debug_;
    FILE *log_file_info_;
    std::string file_name_;
    int pos_;
    int file_size_;
  

private:
    uint8_t *data_byte_;
    uint8_t data_byte_length_;
    // uint16_t entire_section_length_;
    std::vector<section_data> section_list_;
    bool exit_section_;
    int last_psi_pid_;
    ParsePsi parse_psi_;
    uint8_t unit_size_;
	uint8_t offset_;
    // map<int,std::pair<int,std::string>> psi_pid_map_;
};

#endif