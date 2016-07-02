#ifndef STR_PARETO_PARSER_H
#define STR_PARETO_PARSER_H

#include <map>
#include <string>
#include <functional>
#include <regex>

class matches {
public:
    std::smatch data;
    int i(int index);
    float f(int index);
    std::string s(int index);
};

class parser {
private:
    std::map <std::string, std::function<void(matches)>> rules;
public:
    void add(const std::string &command, const std::function<void(matches)> &action);
    void parse(const std::string &command);
};

void strreplace(std::string &str, const std::string &from, const std::string &to);

#endif
