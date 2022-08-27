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
    FormatTable(std::vector<std::string> fields) : m_size(fields.size()) {
        for(auto it = fields.begin(); it != fields.end(); ++it) {
            m_fields.push_back({*it, it->length()});
        }
    }

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

    void addRow(std::vector<std::string> args) {
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
        std::vector<size_t> widths;
        widths.resize(m_fields.size());
        std::transform(m_fields.begin(), m_fields.end(), widths.begin(), [](Field f){ return f.width; });

        printFormat(widths, nullptr, '=');
        std::cout << "|";
        for(size_t i = 0 ; i < m_fields.size(); i++) {
            std::cout << std::setw(m_fields[i].width+1) << std::setfill(' ') << m_fields[i].str << "|";
        }
        std::cout << std::endl;
        printFormat(widths, nullptr, '=');
        for(auto r : rows) {
            printFormat(widths, &r, ' ', '|');
            printFormat(widths);
        }
    }
private:
    size_t m_size;
    std::vector<Field> m_fields;
    std::vector<std::vector<std::string>> rows;

    void printFormat(std::vector<size_t>& widths, std::vector<std::string>* vec = nullptr, char fill = '-', char border = '+', size_t offset = 1) {
        std::cout << border;
        for(size_t i = 0 ; i < widths.size(); i++) {
            std::cout << std::setw(widths[i]+offset) << std::setfill(fill) << ((vec != nullptr) ? vec->at(i) : "") << border;
        }
        std::cout << std::endl;
    }

};
}
#endif // TABLEFORMAT_H
