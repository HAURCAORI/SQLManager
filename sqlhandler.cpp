#include "sqlhandler.h"
#include <iostream>

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

        while(res->next()) {
            for(auto s : fields) {
                std::cout << res->getString(s) << " ";
            }
            std::cout << std::endl;
        }

    } catch (sql::SQLException& e) {
        std::cerr << "Error printing table : " << e.what()<< std::endl;
    }
}



