#ifndef TABLEFORMAT_H
#define TABLEFORMAT_H
#include <iostream>
#include <iomanip>
#include <vector>

namespace Table {

struct Field {
    std::string str;
    size_t width;
};

class FormatTable {
public:
    FormatTable() : m_size(0) {}
    FormatTable(std::initializer_list<std::string> fields) : m_size(fields.size()) {
        for(auto it = fields.begin(); it != fields.end(); ++it) {
            m_fields.push_back({*it, it->length()});
        }
    }

/*
    template<typename T>
    void add(T arg) {

    }

    template<typename T, typename... Types>
    void addRow(T arg, Types... args) {
        size_t size = sizeof...(args) + 1;
        if(size != m_size) {
            throw std::range_error("Arguments size is different.");
        }
        add(args);
    }
*/
    void addRow(std::initializer_list<std::string> args) {
        if(args.size() != m_size) {
            throw std::range_error("Arguments size is different.");
        }
        size_t index = 0;
        std::vector<std::string> ret;
        for(auto it = args.begin(); it != args.end(); ++it) {
            ret.push_back(*it);
            m_fields[index].width = (m_fields[index].width < it->length()) ? it->length() : m_fields[index].width;
            ++index;
        }
        rows.push_back(ret);
    }

    void print() {
        auto printInterval = [&]() {
            std::cout << "+";
            for(size_t i = 0 ; i < m_fields.size(); i++) {
                std::cout << std::setw(m_fields[i].width+6) << std::setfill('-') << "+";
            }
            std::cout << std::endl;
        };

        std::cout << "+";
        for(size_t i = 0 ; i < m_fields.size(); i++) {
            std::cout << std::setw(m_fields[i].width+6) << std::setfill('-') << "+";
        }
        std::cout << std::endl;
        std::cout << "|";
        for(size_t i = 0 ; i < m_fields.size(); i++) {
            std::cout << std::setw(m_fields[i].width+5) << std::setfill(' ') << m_fields[i].str << "|";
        }
        std::cout << std::endl;
        for(auto r : rows) {
            std::cout << "+";
            for(size_t i = 0 ; i < m_fields.size(); i++) {
                std::cout << std::setw(m_fields[i].width+6) << std::setfill('-') << "+";
            }
            std::cout << std::endl;
            std::cout << "|";
            for(size_t i = 0 ; i < m_fields.size(); i++) {
                std::cout << std::setw(m_fields[i].width+5) << std::setfill(' ') << r[i] << "|";
            }
        }
    }
private:
    size_t m_size;
    std::vector<Field> m_fields;
    std::vector<std::vector<std::string>> rows;

};
}
#endif // TABLEFORMAT_H
