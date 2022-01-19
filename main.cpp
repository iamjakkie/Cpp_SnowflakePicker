#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <cstring>
#include <string.h>

void extractError(std::string fn, SQLHANDLE handle, SQLSMALLINT type)
{
    SQLINTEGER i = 0;
    SQLINTEGER native;
    SQLCHAR state[7];
    SQLCHAR text[256];
    SQLSMALLINT len;
    SQLRETURN ret;

    std::cout << "The driver reported the following diagnostics whilst running " << fn << std::endl;

    do
    {
        ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len);
        if (SQL_SUCCEEDED(ret))
        {
            printf("%s:%ld:%ld:%s\n", state, i, native, text);
        }
    } while (ret == SQL_SUCCESS);
}

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
    SQLLEN lenDbName=0;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    SQLRETURN SR;

    std::cout << "Attempting connection " << std::endl;
    // const std::string sfServer = std::getenv("SF_SERVER");
    // const std::string sfLogin = std::getenv("SF_LOGIN");
    // const std::string sfPassword = std::getenv("SF_PASSWORD");
    // std::string connStr = "DRIVER={snowflake_driver};"; //SERVER=" + sfServer + ";UID=" + sfLogin + ";PWD=" + sfPassword + ";";
    
    // strcpy((char *)connectIn, connStr.c_str());
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

    // ret = SQLDriverConnect(dbc, NULL, connectIn, SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_NOPROMPT);
    ret = SQLConnect(dbc, (SQLCHAR *) "snowflake_driver", SQL_NTS, (SQLCHAR *) NULL, SQL_NTS, (SQLCHAR *) NULL, SQL_NTS);

    std::cout << "Connecting ...." << std::endl;
    extractError("SQLConnect", dbc, SQL_HANDLE_DBC);

    if (SR != SQL_SUCCESS && SR != SQL_SUCCESS_WITH_INFO)
    {
        std::cout << "Failed to connect" << std::endl;
    }

    // // ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &shstmt);strcpy((char *)connectIn, connStr.c_str());
    // ret = SQLDriverConnect(dbc, NULL, connectIn, SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_NOPROMPT);

    // std::cout << "Connecting ...." << std::endl;
    // extractError("SQLConnect", dbc, SQL_HANDLE_DBC);

    // if (SR != SQL_SUCCESS && SR != SQL_SUCCESS_WITH_INFO)
    // {
    //     std::cout << "Failed to connect" << std::endl;
    // }

    // if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    // {
    //     std::cout << "Allocated a handle" << std::endl;
    // }

    // ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &shstmt);
    strcpy((char *)SQL_getDbs, "SELECT DATABASE_NAME FROM INFORMATION_SCHEMA.DATABASES");

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &shstmt);
    ret = SQLExecDirect(shstmt, SQL_getDbs, SQL_NTS);
    // ret = SQLPrepare(dbc, SQL_getDbs, SQL_NTS);
    // if(ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO){
    //     extractError("Prepare", dbc, SQL_HANDLE_STMT);
    // }

    // ret = SQLExecute(dbc);
    SQLBindCol(shstmt, 1, SQL_C_CHAR, &dbName, 50, &lenDbName);
    // SQLExecDirect(shstmt, SQL_getDbs, SQL_NTS);
    while(ret != SQL_NO_DATA){
        ret = SQLFetch(shstmt);
        if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO){
            printf("DBNAME: %s\n", dbName);
        }
    }

}