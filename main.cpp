#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <cstring>

void extractError(std::string fn, SQLHANDLE handle, SQLSMALLINT type){
    SQLINTEGER i = 0;
    SQLINTEGER native;
    SQLCHAR state[7];
    SQLCHAR text[256];
    SQLSMALLINT len;
    SQLRETURN ret;

    std::cout << "The driver reported the following diagnostics whilst running "<< fn << std::endl;

    do{
        ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len);
        if(SQL_SUCCEEDED(ret)){
            printf("%s:%ld:%ld:%s\n", state, i, native, text);
        }
    }
    while(ret == SQL_SUCCESS);
}

int main(){
    std::cout << "Starting..." << std::endl;

    SQLHENV env;
    SQLHDBC dbc;
    SQLRETURN ret;
    SQLCHAR outstr[1024], connectIn[50];
    SQLSMALLINT outstrlen;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    SQLRETURN SR;

    std::cout << "Attempting connection " << std::endl;
    const std::string sfServer = std::getenv("SF_SERVER");
    const std::string sfLogin = std::getenv("SF_LOGIN");
    const std::string sfPassword = std::getenv("SF_PASSWORD");
    std::string connStr = "DRIVER={SnowflakeDSIIDriver};SERVER="+sfServer+";UID="+sfLogin+";PWD="+sfPassword+";";;
    strcpy((char*) connectIn, connStr.c_str());
    ret = SQLDriverConnect(dbc, NULL, connectIn, SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_NOPROMPT);

    std::cout << "Connecting ...." << std::endl;
    extractError("SQLConnect", dbc, SQL_HANDLE_DBC);

    if(SR!=SQL_SUCCESS && SR!=SQL_SUCCESS_WITH_INFO){
        std::cout << "Failed to connect" << std::endl;
    } else{
        std::cout << "Connected" <<std::endl;
    }

    return 0;
}