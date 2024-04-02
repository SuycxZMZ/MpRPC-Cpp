#include "mprpcconfig.h"
#include <iostream>
#include <string>

void MprpcConfig::LoadConfigFile(const char * config_file)
{
    FILE * pf = fopen(config_file, "r");
    if (nullptr == pf)
    {
        std::cout << config_file << " is not exist !!!" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (!feof(pf))
    {
        char buf[512];
        fgets(buf, 512, pf);

        // 去掉多余的空格
        std::string str_buf(buf);
        int idx = str_buf.find_first_not_of(' ');
    }
}
std::string MprpcConfig::Load(const std::string & key)
{

}