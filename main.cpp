#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <cstring>
#include <string.h>

int main()
{
    std::cout << "Starting..." << std::endl;

    SQLHENV env;
    SQLHDBC dbc;
    SQLRETURN ret;
    SQLCHAR outstr[1024], connectIn[50];
    SQLSMALLINT outstrlen;

    SQLCHAR SQL_getDbs[150];
    SQLHSTMT shstmt;

    SQLCHAR dbName[50];
    SQLLEN lenDbName = 0;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    SQLRETURN SR;

    std::cout << "Attempting connection " << std::endl;
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
    ret = SQLConnect(dbc, (SQLCHAR *)"snowflake_driver", SQL_NTS, (SQLCHAR *)NULL, SQL_NTS, (SQLCHAR *)NULL, SQL_NTS);
    std::cout << "Connecting ...." << std::endl;
    if (SR != SQL_SUCCESS && SR != SQL_SUCCESS_WITH_INFO)
    {
        std::cout << "Failed to connect" << std::endl;
    }

    strcpy((char *)SQL_getDbs, "SELECT DATABASE_NAME FROM INFORMATION_SCHEMA.DATABASES");

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &shstmt);
    ret = SQLExecDirect(shstmt, SQL_getDbs, SQL_NTS);
    SQLBindCol(shstmt, 1, SQL_C_CHAR, &dbName, 50, &lenDbName);
    while (ret != SQL_NO_DATA)
    {
        ret = SQLFetch(shstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            printf("DBNAME: %s\n", dbName);
        }
    }
}