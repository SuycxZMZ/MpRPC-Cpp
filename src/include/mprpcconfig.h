#ifndef MPRPCCONFIG_H
#define MPRPCCONFIG_H

#include <unordered_map>
#include <string>

void removeSpaces(std::string& str);

class MprpcConfig
{
public:
    void LoadConfigFile(const char * config_file);
    std::string Load(const std::string & key);
private:
    std::unordered_map<std::string, std::string> m_configMap;
};


#endif