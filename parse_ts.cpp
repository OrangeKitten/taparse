#include "parse_ts.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <algorithm>
#include "log.h"
#include "parse_psi.h"
#include <cstring>
void Parse_Ts::GetDataFromFile()
{
    ts_file_ = fopen(file_name_.c_str(), "rb");
    if (ts_file_ != NULL)
    {
        // 获取文件大小
        fseek(ts_file_, 0, SEEK_END);
        file_size_ = ftell(ts_file_);
        fseek(ts_file_, 0, SEEK_SET);

        // 分配足够的内存来存储文件内容
        file_content_ = (uint8_t *)malloc(file_size_ + 1);

        if (file_content_ != NULL)
        {
            // 读取文件内容到动态分配的内存中
            fread(file_content_, 1, file_size_, ts_file_);
            // 释放动态分配的内存
        }
        else
        {
            printf("内存分配失败。\n");
        }

        // 关闭文件
        fclose(ts_file_);
    }
    else
    {
        printf("无法打开文件。\n");
    }
}
bool Parse_Ts::ParseInit()
{
    // add eit process thread
    log_set_level(3);
    log_set_quiet(1);

    log_file_info_ = fopen("./log_info.txt", "w");
    log_file_debug_ = fopen("./log_debug.txt", "w");
    if (log_file_info_ == NULL || log_file_debug_ == NULL)
        return -1;
    log_add_fp(log_file_info_, LOG_INFO);
    log_add_fp(log_file_debug_, LOG_DEBUG);
    return true;
}

int Parse_Ts::ParseTsHead(uint8_t *PaketHead)
{

    int offset = 0;

    ts_head_.sync_byte = PaketHead[0];
    // log_debug("PaketHead[0] = %0x", PaketHead[0]);
    ts_head_.transport_error_indicator = (PaketHead[1] & 0x80) >> 7;
    ts_head_.payload_unit_start_indicator = (PaketHead[1] & 0x40) >> 6;
    ts_head_.transport_priority = (PaketHead[1] & 0x20) >> 5;
    ts_head_.PID = ((PaketHead[1] & 0x1f) << 8) | PaketHead[2];
    ts_head_.transport_scrambling_control = (PaketHead[3] & 0x60) >> 6;
    ts_head_.adaptation_field_control = (PaketHead[3] & 0x30) >> 4;
    ts_head_.continuity_counter = (PaketHead[3] & 0x0f);

    offset += 4;
    if (!ts_head_.adaptation_field_control)
    {
        /* fix me */
        return 0;
    }
    // if (ts_head_.PID == 0x11)
    // {
    //     Dump();
    // }
    //Dump();
    //  Debug(PaketHead, 6);
    //   exit(0);
    //   注意是0b 不是0X 0b表示二进制

    if (ts_head_.adaptation_field_control == 0b10 || ts_head_.adaptation_field_control == 0b11)
    {
        // TODO
        //  ParseAdaptationField();
        // log_debug("&data_byte_ = %0x", &data_byte_);
        offset += PaketHead[4] + 1;
    }
    if (ts_head_.adaptation_field_control == 0b01 || ts_head_.adaptation_field_control == 0b11)
    {
        data_byte_ = PaketHead + offset;
        // log_debug("&data_byte_ = %0x", &data_byte_);
    }
    // log_info("offset = %0x", offset);
    data_byte_length_ = TS_PACKET_SIZE - offset;
    if (data_byte_length_ <= 0)
    {
        return 0;
    }
    return offset;
}

int Parse_Ts::CheckHead(uint8_t *temp_file_content_, int offset)
{
    for (int i = 0; i < file_size_ - pos_; i++)
    {
        // log_info("temp_file_content_[i+offset] = %0x,temp_file_content_[unit_size_]=%0x",temp_file_content_[i+offset],temp_file_content_[i+unit_size_]);
        if (temp_file_content_[i + offset] == 0x47 && temp_file_content_[i + unit_size_ + offset] == 0x47)
        {

            return i;
        }
    }
    return 0;
}
void Parse_Ts::Dump() const
{
    log_info("sync_byte = %0x\n", ts_head_.sync_byte);
    log_info("transport_error_indicator = %0x\n", ts_head_.transport_error_indicator);
    log_info("payload_unit_start_indicator = %0x\n", ts_head_.payload_unit_start_indicator);
    log_info("transport_priority = %0x\n", ts_head_.transport_priority);
    log_info("PID = %0x\n", ts_head_.PID);
    log_info("continuity_counter = %0x\n", ts_head_.continuity_counter);
    log_info("adaptation_field_control = %0x\n", ts_head_.adaptation_field_control);
}
bool Parse_Ts::ParseTS()
{

    GetDataFromFile();
    // log_info("file_content_ = %p", file_content_);
    unit_size_ = check_TS_unit_size();
    // log_info("unit_size_ = %0x", unit_size_);
    offset_ = unit_size_ == TTS_PACKET_SIZE ? 4 : 0;
    // int offset = 0;
    // 找到ts包头的位置
    // log_info("offset_ = %0x", offset_);
    uint8_t *temp_file_content_ = file_content_;
    pos_ = CheckHead(temp_file_content_, offset_);
    // log_info("pos_ = %0x", pos_);
    int headsize = 0;
    temp_file_content_ = temp_file_content_ + pos_;
    // exit(0);
    // 每次只处理188/192/204字节数据
    while (pos_ < file_size_)
    {
        headsize = ParseTsHead(temp_file_content_ + offset_);
        if (ts_head_.PID == 0x11)
            log_debug("headsize = %d", headsize);
        if (ts_head_.PID != 0X1FFF && data_byte_ && headsize)
        {
            ParsePayload();
            // for(auto &section_data:section_list_) {
            // if (section_data.is_entire_data)
            // {
            //     parse_psi_.decode(section_data.section_buffer.data(), section_data.section_buffer.size());
            //     section_list_.section_data.clear();
            // }
            // }

            
            if (section_list_.size())
            {
                
                std::vector<section_data>::iterator itor;
                for (itor = section_list_.begin(); itor != section_list_.end(); )
                {
                    if ((*itor).is_entire_data)
                    {
                        log_debug("section_list_.size() = %0x", section_list_.size());
                        parse_psi_.decode((*itor).section_buffer.data(), (*itor).section_buffer.size());
                        itor = section_list_.erase(itor);
                    } else {
                        itor++;
                    }
                }
            }
        }
        temp_file_content_ = temp_file_content_ + unit_size_;
        pos_ += unit_size_;
    }

    return true;
}

void Parse_Ts::ParsePayload()
{

    exit_section_ = false;
    if (ts_head_.PID == 0x11)
        log_debug("1");
    // TODO parse PES
    int packet_start_code_prefix = data_byte_[0] << 16 | data_byte_[1] << 8 | data_byte_[2];
    // log_debug("packet_start_code_prefix = %0x", packet_start_code_prefix);
    if (packet_start_code_prefix == 0x000'001)
    {
        // log_debug("current ts include pes");
        return;
    }
    //  if(ts_head_.PID != 0x11)   return;
    // TODO sdt解析不出来 是因为现在的逻辑是 如果当前拼包的是EIT那么就会把EIT信息全部拿到之后才会去获取之后的section，这时候SDT会由于上一包没有拼完而被过滤
    // log_debug("payload_unit_start_indicator = 0x%0x ",ts_head_.payload_unit_start_indicator );
    if (ts_head_.payload_unit_start_indicator == 1) // 代表当前ts包，包含pes/psi的包头
    {

        const uint8_t pointer_field = data_byte_[0];
        // TODO
        //  if (pointer_field != 0 || !section_buffer_.empty())
        //  {

        // }
        int section_start_index = pointer_field + 1;
        section_data section_buffer;
        memset(&section_buffer, 0, sizeof(section_data));
        do
        {

            int section_offset = section_start_index;
            const uint8_t table_id = data_byte_[section_offset];
            if (table_id == 0xff)
                return;
            section_buffer.PID = ts_head_.PID;
            section_buffer.TABLE_ID = table_id;
            log_info("table_id = %0x ", table_id);
            // if (table_id == 0xff)
            //     Dump();
            section_offset++;
            section_buffer.entire_section_length_ = 3 + ((data_byte_[section_offset] & 0x0f) << 8 | data_byte_[++section_offset]);
            // log_debug("entire_section_length_ = %0x data_byte_length_ = %0x section_start_index = %0x", entire_section_length_, data_byte_length_, section_start_index);
            int end = std::min(static_cast<int>(data_byte_length_), section_buffer.entire_section_length_ + section_start_index);
            section_buffer.section_buffer.assign(&data_byte_[section_start_index], &data_byte_[end]);

            if (section_buffer.section_buffer.size() == section_buffer.entire_section_length_)
            {
                log_debug("section_buffer.section_buffer.size() = %0x", section_buffer.section_buffer.size());
                section_buffer.is_entire_data = true;
            }
            section_list_.push_back(section_buffer);
            section_start_index += section_buffer.entire_section_length_;
            last_psi_pid_ = ts_head_.PID;
        } while (section_start_index <= data_byte_length_);
    }
    else
    {
        if (packet_start_code_prefix == 0x000'001)
        {
            // TODO pes parse flow
            log_debug("current ts include pes");
            return;
        }
        for (auto &section_buffer : section_list_)
        {
            if (section_buffer.PID == ts_head_.PID)
            {
                int endd = std::min(static_cast<size_t>(data_byte_length_), section_buffer.entire_section_length_ - section_buffer.section_buffer.size());
                section_buffer.section_buffer.insert(section_buffer.section_buffer.end(), data_byte_, data_byte_ + endd);
                // log_debug("entire_section_length_ = %0x data_byte_length_ = %0x", entire_section_length_, data_byte_length_);
                // log_debug("section_buffer_.size() = %0x", section_buffer_.size());
                if (section_buffer.section_buffer.size() == section_buffer.entire_section_length_)
                {
                    // section_list_.push_back(section_buffer_);
                    // section_buffer_.clear();
                    section_buffer.is_entire_data = true;
                }
            }
        }
    }
}
int Parse_Ts::get_file_size()
{
    return file_size_;
}

uint8_t Parse_Ts::check_TS_unit_size()
{
    int buf_size = 1024;
    uint8_t *tmp_buff = file_content_;
    auto sync_count = [buf_size, &tmp_buff](const size_t unit_size, const ptrdiff_t offset = 0)
    {
        auto cnt = 0;
        for (auto i = 0; i < buf_size; i += unit_size)
        {
            if (tmp_buff[i + offset] == 0x47)
                ++cnt;
        }
        return cnt;
    };

    const auto ts_cnt = sync_count(TS_PACKET_SIZE);
    const auto tts_cnt = sync_count(TTS_PACKET_SIZE, TTS_PACKET_SIZE - TS_PACKET_SIZE);
    const auto fects_cnt = sync_count(TS_PACKET_FECTS_SIZE);

    return (ts_cnt > tts_cnt && ts_cnt > fects_cnt) ? TS_PACKET_SIZE : (tts_cnt > fects_cnt) ? TTS_PACKET_SIZE
                                                                                             : TS_PACKET_FECTS_SIZE;
}
void Parse_Ts::Debug(uint8_t *data, int length)
{
    for (int i = 0; i < length; i++)
    {
        log_debug("%0x", data[i]);
    }
}
// PacketType Parse_Ts::get_packet_type(int pid) {

// }

// void Parse_Ts::DispatchDeconde(uint8_t*section_data,int length) {
//     if(section_data==nullptr) return;
//     const uint8_t table_id = section_data[0];
//     switch (table_id)
//     {
//     case /* constant-expression */:
//         /* code */
//         break;

//     default:
//         break;
//     }

// }