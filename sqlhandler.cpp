#include "sqlhandler.h"
#include <iostream>
#include "tableformat.h"

SQLHandler::SQLHandler() {}

SQLHandler::~SQLHandler()
{
    stop_all = true;
    cv_job_q_.notify_all();
    disconnect();
}

/**
 * @brief connect
 * @details SQL 서버와 연결
 * @param url SQL connection URL
 * @param user Access ID
 * @param password Acesss Password
 */
void SQLHandler::connect(std::string url, std::string user, std::string password)
{
    clear();
    EnqueueJob([this, url, user, password](){ _connect(url,user,password); });
}

void SQLHandler::_connect(std::string url, std::string user, std::string password)
{
    try {
        driver = sql::mariadb::get_driver_instance();

        sql::Properties properties({
                                       {"user", user},
                                       {"password", password}
                                   });

        conn = std::unique_ptr<sql::Connection>(driver->connect(url, properties));
        stmnt = std::shared_ptr<sql::Statement>(conn->createStatement());
        std::cout << "[sql] A connection successfully established." << std::endl;
        isconnected = true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[sql] Error Connecting to the database: " << e.what() << std::endl;
    }
}

/**
 * @brief disconnect
 * @details Close connection
 */
void SQLHandler::disconnect() noexcept
{
    if(conn) {
        conn->close();
    }
    isconnected = false;
    std::cout << "[sql] Disconnected." << std::endl;
}

bool SQLHandler::connected() {
    return isconnected;
}

/**
 * @brief run
 * @details Queue 기반 백그라운드 쓰레드 작업 수행
 */
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

/**
 * @brief stop
 * @details 백그라운드 쓰레드 종료
 */
void SQLHandler::stop() noexcept
{
    stop_all = true;
    cv_job_q_.notify_all();
}

/**
 * @brief clear
 * @details Queue 초기화
 */
void SQLHandler::clear() noexcept
{
    std::queue<std::function<void()>> queue;
    swap(jobs_,queue);
}

void SQLHandler::printTable(std::string table, std::vector<std::string> fields)
{
    EnqueueJob([this, table, fields](){ _printTable(table, fields); });
}

void SQLHandler::testSlot()
{

}

/**
 * @brief printTable
 * @param table
 * @param fields
 */
void SQLHandler::_printTable(std::string table, std::vector<std::string> fields)
{
    if(!isconnected) { return; }
    try {
        std::string query = "SELECT";

        for(auto it = fields.begin(); it != fields.end(); ++it) {
            query+= " " + *it + ",";
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
        std::cerr << "[sql] Error printing table : " << e.what()<< std::endl;
    }
}

void SQLHandler::_insert()
{
    std::shared_ptr<sql::PreparedStatement> pstmnt(
                conn->prepareStatement(
                   "INSERT INTO test.contacts(first_name, last_name, email) VALUES (?, ?, ?)"
                )
             );


}



