 #ifndef __PARSEDESCRIPTOR__
 #define __PARSEDESCRIPTOR__

// struct VideoStreamDescriptor
// {
// 	uint8_t descriptor_tag; //0x02
// 	uint8_t descriptor_length;
// 	int8_t  multiple_frame_rate_flag;
// 	uint8_t frame_rate_code;
// 	int8_t  MPEG_1_only_flag;
// 	int8_t  constrained_parameter_flag;
// 	int8_t  still_picture_flag;

// 	uint8_t profile_and_level_indication;
// 	uint8_t chroma_format;
// 	int8_t  frame_rate_extension_flag;

// 	bool parse(const uint8_t* p);
// };

// struct RegistrationDescriptor
// {
// 	uint8_t  descriptor_tag;
// 	uint8_t  descriptor_length;
// 	uint32_t format_indentifier;
// 	std::vector<uint8_t> additional_identification_info;

// 	bool parse(const uint8_t* p);
// };

// struct DataStreamAlignmentDescriptor
// {
// 	uint8_t descriptor_tag;  //0x06
// 	uint8_t descriptor_length;
// 	uint8_t alignment_type;

// 	bool parse(const uint8_t* p);
// };

// struct AVCVideoDescriptor
// {
// 	uint8_t descriptor_tag; //0x28
// 	uint8_t descriptor_length;
// 	int8_t  profile_idc;
// 	int8_t  constraint_set0_flag;
// 	int8_t  constraint_set1_flag;
// 	int8_t  constraint_set2_flag;
// 	int8_t  constraint_set3_flag;
// 	int8_t  constraint_set4_flag;
// 	int8_t  constraint_set5_flag;
// 	int8_t  AVC_compatible_flag;
// 	uint8_t level_idc;
// 	int8_t  AVC_still_present;
// 	int8_t  AVC_24_hour_picture_flag;
// 	int8_t  Frame_Packing_SEI_not_present_flag;

// 	bool parse(const uint8_t* p);
// };
// struct AVCTimingAndHRFDescriptor
// {
// 	uint8_t   descriptor_tag; //0x2A
// 	uint8_t   descriptor_length;
// 	int8_t    hrd_management_valid_flag;
// 	int8_t    picture_and_timing_info_present;
// 	int8_t    _90kHz_flag;
// 	uint32_t  N;
// 	uint32_t  K;
// 	uint32_t  num_units_in_tick;
// 	int8_t  fixed_frame_rate_flag;
// 	int8_t  temporal_poc_flag;
// 	int8_t  picture_to_display_conversion_flag;

// 	bool parse(const uint8_t* p);
// };

class ParseDescriptor{
    public:
    ParseDescriptor() = default;
    ~ParseDescriptor() = default;
    void Parse_Descriptor();
    void Decode(uint8_t*data,int length);
    public:
    uint8_t descriptor_tag;   
    uint16_t descriptor_length;  
};
 #endif