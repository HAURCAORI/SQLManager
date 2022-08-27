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
    void init(std::string url, std::string user, std::string password);
    void run();
    void printTable(std::string table, std::initializer_list<std::string> fields);

public slots:
    bool printData();

private:
    //Threading
    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(F&& f, Args&&... args);

    std::queue<std::function<void()>> jobs_;
    std::condition_variable cv_job_q_;
    std::mutex m_job_q_;
    bool stop_all;

    //MariaDB


    sql::Driver* driver;
    std::unique_ptr<sql::Connection> conn;
    std::shared_ptr<sql::Statement> stmnt;


};


template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> SQLHandler::EnqueueJob(F&& f, Args&&... args) {
    if (stop_all) {
        throw std::runtime_error("ThreadPool 사용 중지됨");
    }

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
