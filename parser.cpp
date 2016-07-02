#include "parser.h"

#include <iostream>
#include <sstream>

int matches::i(int index) {
    int result;
    std::stringstream ss(data[index]);
    ss >> result;
    return result;
}

float matches::f(int index) {
    float result;
    std::stringstream ss(data[index]);
    ss >> result;
    return result;
}

std::string matches::s(int index) {
    std::string result;
    std::stringstream ss(data[index]);
    ss >> result;
    return result;
}

void parser::add(const std::string &command, const std::function<void(matches)> &action) {
    std::string result = "^";
    result += command;
    result += "$";

    strreplace(result, "%i", "([+-]?\\d+)");
    strreplace(result, "%f", "([+-]?(?=\\d*[.eE])(?=\\.?\\d)\\d*\\.?\\d*(?:[eE][+-]?\\d+)?)");
    strreplace(result, "%s", "(\\S+)");
    strreplace(result, "%*", "(.+)");

    rules[result] = action;
}

void parser::parse(const std::string &command) {
    if(command.size() == 0) return;

    bool unknown = true;
    for(auto it=rules.begin();it!=rules.end();it++) {
        matches m;
        if(std::regex_match(command, m.data, std::regex(it->first))) {
            it->second(m);
            unknown = false;
            break;
        }
    }

    if(unknown) {
        std::cout << "Unknown command " << std::endl;
    }
}

void strreplace(std::string &str, const std::string &from, const std::string &to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
