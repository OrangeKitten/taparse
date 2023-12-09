
#include <string>
#include <iostream>
#include "parse_ts.h"
int main(int argc, char *argv[])
{
    // if (argc < 2)
    // {
    //     std::cout << "please inset parse ts file name" << std::endl;
    //      return 0;
    // }
   // std::string file_name(argv[1]);
       //std::string file_name("Chongqing_618_20171109_0034.ts");
//std::string file_name("Chengdu_762_20171107_2328.ts");
//  std::string file_name("dmx_all188.ts");
 std::string file_name("DTMB_subtitle.ts");


    // init log/thread
    std::cout << "file name = " << file_name<<std::endl;
    Parse_Ts parse_ts(file_name);
    bool ret = parse_ts.ParseInit();
    if (ret == false)
    {
        std::cout << "ParseInit failed" << std::endl;
       
    }

    ret = parse_ts.ParseTS();
    if (ret == false)
    {
        std::cout << "ParseTS failed" << std::endl;
    }
    return 0;
}
