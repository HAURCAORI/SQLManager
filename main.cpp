#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include "mariadb/conncpp.hpp"

#include <qthreadpool.h>
#include "tableformat.h"
#include "datatable.h"

void printContacts(std::shared_ptr<sql::Statement> &stmnt)
{
   try {
      // Execute SELECT Statement
      std::unique_ptr<sql::ResultSet> res(
            stmnt->executeQuery("SELECT * FROM test")
         );

      // Loop over Result-set
      while (res->next())
      {
         // Retrieve Values and Print Contacts
         std::cout << "- "
            << res->getString("id")
            << " "
            << res->getString("value")
            << std::endl;
      }
   }

   // Catch Exception
   catch (sql::SQLException& e) {
      std::cerr << "Error printing contacts: "
         << e.what() << std::endl;
   }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //w.add();


/*
    try {
          // Instantiate Driver
          sql::Driver* driver = sql::mariadb::get_driver_instance();

          // Configure Connection
          // The URL or TCP connection string format is
          // ``jdbc:mariadb://host:port/database``.
          //omen-home.iptime.org
          //203.251.63.137
          sql::SQLString url("tcp://203.251.63.137:5000/test");

          // Use a properties map for the user name and password
          sql::Properties properties({
                {"user", "guest"},
                {"password", "1234"}
             });

          // Establish Connection
          // Use a smart pointer for extra safety
          std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

          std::shared_ptr<sql::Statement> stmnt(conn->createStatement());

          printContacts(stmnt);

          conn->close();
       }
       catch (sql::SQLException& e) {
          std::cerr << "Error Connecting to the database: " << e.what() << std::endl;

       }
*/

    return a.exec();
}
