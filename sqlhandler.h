#ifndef SQLHANDLER_H
#define SQLHANDLER_H

#include <QThread>

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>

#include "mariadb/conncpp.hpp"


class SQLHandler : public QThread
{
    Q_OBJECT

public:
    SQLHandler();
    ~SQLHandler();

    void run();
    void stop() noexcept;
    void clear() noexcept;

    void connect(std::string url, std::string user, std::string password);
    void disconnect() noexcept;

    bool connected();





    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(F&& f, Args&&... args);


    void printTable(std::string table, std::vector<std::string> fields);

    //DML
    bool Insert();
    bool Delete();
    bool Update();
    bool Select();

    //DDL
    bool Create();
    bool Drop();
    bool Altar();

public slots:
    void testSlot();
private:
    //Declaration of Function
    void _connect(std::string url, std::string user, std::string password);
    void _printTable(std::string table, std::vector<std::string> fields);

    //Threading 관련 변수
    std::queue<std::function<void()>> jobs_;
    std::condition_variable cv_job_q_;
    std::mutex m_job_q_;
    bool stop_all = false;

    //MariaDB
    sql::Driver* driver;
    std::unique_ptr<sql::Connection> conn = nullptr;
    std::shared_ptr<sql::Statement> stmnt = nullptr;

    //Global
    bool isconnected = false;

};


template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> SQLHandler::EnqueueJob(F&& f, Args&&... args) {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto job = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> job_result_future = job->get_future();
    {
        std::lock_guard<std::mutex> lock(m_job_q_);
        jobs_.push([job]() { (*job)(); });
    }
    cv_job_q_.notify_one();

    return job_result_future;
}

#endif // SQLHANDLER_H
