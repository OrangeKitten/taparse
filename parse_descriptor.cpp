#include "parse_descriptor.h"
#include "log.h"
#include <iostream>
void Service_Descriptor::Parse(uint8_t *data)
{

    descriptor_tag = data[0];
    descriptor_length = data[1];
    servrice_type = data[2];
    service_provider_length = data[3];
    for (int i = 0; i < service_provider_length; i++)
    {
        service_provider_name += data[4 + i];
    }
    service_name_length = data[service_provider_length + 4];

    for (int i = 0; i < service_name_length; i++)
    {
        service_name+= data[5 + service_provider_length + i];
    }


}
void Service_Descriptor::Dump()
{
    log_info("descriptor_tag = %0x", descriptor_tag);
    log_info("descriptor_length = %0x", descriptor_length);
    log_info("servrice_type = %0x", servrice_type);
    log_info("service_provider_length = %0x", service_provider_length);
    log_info("service_provider_name = %s", service_provider_name.c_str());
    log_info("service_name_length = %0x", service_name_length);
    log_info("service_name = %s", service_name.c_str());
}
void ParseDescriptor::Parse_Descriptor(uint8_t *data, int length)
{
    int decode_index = 0;
    uint8_t *tmp_data = data;
    while (decode_index < length)
    {
        uint8_t descriptor_tag = tmp_data[0];
        uint8_t descriptor_length = tmp_data[1];
        log_info("descriptor_tag = %0x", descriptor_tag);
        switch (descriptor_tag)
        {
        case 0x48:
        {
            descriptor_data_.service_descriptor = std::make_unique<Service_Descriptor>();
            descriptor_data_.service_descriptor->Parse(tmp_data);
            break;
        }
        default:
            break;
        }

        decode_index += descriptor_length + 2;
        tmp_data += decode_index;
    }
}
