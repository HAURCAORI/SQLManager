#include "sqlhandler.h"
#include <iostream>
#include "tableformat.h"

SQLHandler::SQLHandler()
{
    stop_all = false;
}

SQLHandler::~SQLHandler()
{
    stop_all = true;
    cv_job_q_.notify_all();
    conn->close();
}

void SQLHandler::init(std::string url, std::string user, std::string password)
{
    try {
        driver = sql::mariadb::get_driver_instance();

        sql::Properties properties({
                                       {"user", user},
                                       {"password", password}
                                   });

        conn = std::unique_ptr<sql::Connection>(driver->connect(url, properties));
        stmnt = std::shared_ptr<sql::Statement>(conn->createStatement());
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error Connecting to the database: " << e.what() << std::endl;
    }
}

void SQLHandler::run()
{
    while (true) {
        std::unique_lock<std::mutex> lock(m_job_q_);
        cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });
        if (stop_all && this->jobs_.empty()) {
            return;
        }

        std::function<void()> job = std::move(jobs_.front());
        jobs_.pop();
        lock.unlock();

        job();
    }
}

bool SQLHandler::printData()
{
    std::cout << "Hello World" << std::endl;
    return true;
}

void SQLHandler::printTable(std::string table, std::initializer_list<std::string> fields)
{
    try {
        std::string query = "SELECT";
        for(auto s : fields) {
            query.append(" " + s + ",");
        }
        query.pop_back();
        query.append(" FROM ");
        query.append(table);

        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(query));
        Table::FormatTable ft{fields};

        while(res->next()) {
            std::vector<std::string> vec;
            for(auto s : fields) {
                vec.emplace_back(std::string(res->getString(s).c_str()));
            }
            ft.addRow(vec);
        }
        ft.print();
    } catch (sql::SQLException& e) {
        std::cerr << "Error printing table : " << e.what()<< std::endl;
    }
}



