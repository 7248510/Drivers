//https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-apps-windows-visual-studio.html
//Reference to the same process with Boost
//cl /EHsc /I C:\boost_1_75_0\ -D _WIN32_WINNT=0x0601 boostTalk.cc /link /LIBPATH:C:\boost_1_75_0\stage\lib

//H:\ConnectorC++
//cl /EHsc /sdl /W3 /I H:\ConnectorC++\include\jdbc connect.cc H:\ConnectorC++\lib64\vs14/*
/* Standard C++ includes */
//https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-examples-complete-example-2.html
#include <stdlib.h>
#include <iostream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
//W4 outputs an algorithm suggestion/bug in msvc's algorithm include file
//cl /EHsc /sdl /W3 /I H:\ConnectorC++64\include\jdbc connect.cc H:\ConnectorC++64\lib64\vs14\*   = 64BIT
//cl /EHsc /sdl /W3 /I H:\ConnectorC++86\include\jdbc connect.cc H:\ConnectorC++86\lib\vs14\*     = x86/32BIT
//MAKE SURE YOU'RE USING THE RIGHT COMPILER ARCH. If your MySQL Install is x64 the compiler has to be x64.
//I copied the connector files(located in program files/mysql(x64 version)| program files(x86) = x86 version to another folder to make CLI arguments easier.
//The library files can be moved anywhere. Because these are not static binaries they have dll deps.

int main(void)
{
    std::cout << std::endl;
    std::cout << "Let's have MySQL count from 10 to 1..." << std::endl;
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;
        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "root"); //Change the username and password!
        /* Connect to the MySQL test database */
        con->setSchema("test"); //database name
        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS test"); 
        stmt->execute("CREATE TABLE test(id INT)");
        delete stmt;
        /* '?' is the supported placeholder syntax */
        pstmt = con->prepareStatement("INSERT INTO test(id) VALUES (?)");
        for (int i = 1; i <= 10; i++) {
            pstmt->setInt(1, i);
            pstmt->executeUpdate();
        }
        delete pstmt;
        /* Select in ascending order */
        pstmt = con->prepareStatement("SELECT id FROM test ORDER BY id ASC");
        res = pstmt->executeQuery();
        /* Fetch in reverse = descending order! */
        res->afterLast();
        while (res->previous()) {
            std::cout << "\t... MySQL counts: " << res->getInt("id") << std::endl;
        }
        delete res;
        delete pstmt;
        delete con;
    } 
    catch (sql::SQLException &errorCode) {
        std::cout << "ERROR";
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << errorCode.what();
        std::cout << " (MySQL error code: " << errorCode.getErrorCode();
        std::cout << ", SQLState: " << errorCode.getSQLState() << " )" << std::endl;
    }
    std::cout << std::endl;
    return EXIT_SUCCESS;
}