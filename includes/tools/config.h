#pragma once

#include <string>
#include <map>
#include <mutex>

class Config
{
private:
    static Config* instance;
    static std::mutex mutex;
public:
    static Config* get_instance();

private:
    std::map<std::string, std::map<std::string, std::string>> data;

    Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

public:
    bool load(const std::string & filePath);
    bool save(const std::string & filePath) const;

    [[nodiscard]] std::string get(const std::string & section, const std::string & key, const std::string & defaultValue = "") const;
    [[nodiscard]] double get_double(const std::string& section, const std::string& key, double defaultValue = 0.0) const;
    [[nodiscard]] int get_int(const std::string& section, const std::string& key, int defaultValue = 0) const;

    void set(const std::string & section, const std::string & key, const std::string & value);
};
