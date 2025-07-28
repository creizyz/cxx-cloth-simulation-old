#include "tools/config.h"
#include "tools/toolbox.h"

#include <fstream>
#include <iostream>

// static member initialization
Config * Config::instance = nullptr;
std::mutex Config::mutex;

Config* Config::get_instance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr)
    {
        instance = new Config();
    }
    return instance;
}

bool Config::load(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    std::string section;
    std::string line;
    while (std::getline(file, line))
    {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Skip comments and empty lines
        if (line.empty() || line[0] == '#' || line[0] == ';')
        {
            continue;
        }

        // Handle section headers
        if (line[0] == '[' && line.back() == ']')
        {
            section = line.substr(1, line.length() - 2);
        }
        else
        {
            size_t eqPos = line.find('=');
            if (eqPos != std::string::npos)
            {
                std::string key = line.substr(0, eqPos);
                std::string value = line.substr(eqPos + 1);

                // Trim both key and value
                key.erase(0, key.find_first_not_of(" \t\r\n"));
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);

                data[Toolbox::to_lower(section)][Toolbox::to_lower(key)] = value;
            }
        }
    }

    file.close();
    return true;
}

bool Config::save(const std::string& filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }

    for (const auto& sectionPair : data)
    {
        const std::string& section = sectionPair.first;
        file << "[" << section << "]\n";

        for (const auto& kv : sectionPair.second)
        {
            file << kv.first << "=" << kv.second << "\n";
        }

        file << "\n";
    }

    file.close();
    return true;
}

std::string Config::get(const std::string& section, const std::string& key, const std::string& defaultValue) const
{
    auto secIt = data.find(Toolbox::to_lower(section));
    if (secIt != data.end())
    {
        auto keyIt = secIt->second.find(Toolbox::to_lower(key));
        if (keyIt != secIt->second.end())
        {
            return keyIt->second;
        }
    }
    return defaultValue;
}

double Config::get_double(const std::string& section, const std::string& key, double defaultValue) const
{
    const auto value = get(section, key, "");
    if (value.empty())
    {
        return defaultValue;
    }

    try
    {
        return std::stod(value);
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Warning: Failed to convert key '" << key
                  << "' in section '" << section << "' to double. Returning default value." << std::endl;
    }
    catch (const std::out_of_range &)
    {
        std::cerr << "Warning: Value of key '" << key
                  << "' in section '" << section << "' is out of range for double. Returning default value." << std::endl;
    }
    return defaultValue;

}

int Config::get_int(const std::string& section, const std::string& key, int defaultValue) const
{
    const auto value = get(section, key, "");
    if (value.empty())
    {
        return defaultValue;
    }

    try
    {
        return std::stoi(value);
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Warning: Failed to convert key '" << key
                  << "' in section '" << section << "' to integer. Returning default value." << std::endl;
    }
    catch (const std::out_of_range &)
    {
        std::cerr << "Warning: Value of key '" << key
                  << "' in section '" << section << "' is out of range for integer. Returning default value." << std::endl;
    }
    return defaultValue;

}

void Config::set(const std::string& section, const std::string& key, const std::string& value)
{
    data[Toolbox::to_lower(section)][Toolbox::to_lower(key)] = value;
}
