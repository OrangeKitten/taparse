#include "parse_psi.h"
#include "parse_pat.h"
#include "parse_pmt.h"
#include "parse_sdt.h"
#include "log.h"
#include <memory>

void ParsePsi::decode(uint8_t *section_data, int length) {
  if (section_data == nullptr)
    return;
  const uint8_t table_id = section_data[0];
  if(table_id == 0xff ) {
    return;
  }
   //log_info("table_id = %0x ",table_id);
  switch (table_id) {
  case 0x00: {
    std::unique_ptr<ParsePAT> parse_pat = std::make_unique<ParsePAT>();
    parse_pat->Decode(section_data, length);
    break;
  }
//   case 0x01: {
//     std::unique_ptr<ParseCAT> parse_pat = std::make_unique<ParseCAT>();
//     parse_pat->Decode(section_data, length);
//     break;
//   }

//   case 0x02: {
//     std::unique_ptr<ParsePMT> parse_pmt = std::make_unique<ParsePMT>();
//     parse_pmt->Decode(section_data, length);
//     break;
//   }

//   case 0x03: {
//     std::unique_ptr<ParseTSDT> parse_pat =
//         std::make_unique<ParsParseTSDTePAT>();
//     parse_pat->Decode(section_data, length);
//     break;
//   }
//   case 0x40:
//   case 0x41: {
//     std::unique_ptr<ParseNIT> parse_pat = std::make_unique<ParseNIT>();
//     parse_pat->Decode(section_data, length);
//     break;
//   }
  case 0x42:
  case 0x46: {
    log_info(" 42");
    std::unique_ptr<ParseSDT> parse_sdt = std::make_unique<ParseSDT>();
    parse_sdt->Decode(section_data, length);
    break;
  }
    #if 0 
  case 0x4A: {
    std::unique_ptr<ParseBAT> parse_pat = std::make_unique<ParsePAT>();
    parse_pat->Decode(section_data, length);
    break;
  }
  case 0x70: {
    std::unique_ptr<ParseTDT> parse_pat = std::make_unique<ParseTDT>();
    parse_pat->Decode(section_data, length);
    break;
  }
  case 0X73: {
    std::unique_ptr<ParseTOT> parse_pat = std::make_unique<ParseTOT>();
    parse_pat->Decode(section_data, length);
    break;
  }
#endif
  default:
    // if (table_id >= 0x4e && table_id <= 0x6f) {
    //   std::unique_ptr<ParseEIT> parse_pat = std::make_unique<ParseEIT>();
    //   parse_eit->Decode(section_data, length);
    // } else {
    //   log_info("table_id = %0x  don't parse", table_id);
    // }
    break;
  }
}
