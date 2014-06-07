#ifndef CONFIGMANAGER_H_INCLUDED
#define CONFIGMANAGER_H_INCLUDED
#include <map>
#include <string>
#include <boost/variant.hpp>

typedef boost::variant<int32_t, std::string, double> Variant;
class ConfigManager
{
public:
    //ConfigManager(){}
    //~ConfigManager(){}
    template <typename ReturnType>
    ReturnType getConfigValue(const std::string& optionName) const
    {
        const auto it = configMap.find(optionName);
        if(it == configMap.end())
            throw;
        return boost::get<ReturnType>(it->second);
    }

    void loadConfig();
private:
    std::map<std::string, Variant> configMap;
};

#endif // CONFIGMANAGER_H_INCLUDED
