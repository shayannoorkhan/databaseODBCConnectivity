// SQLTesting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

//include the below additional libraries
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include<odbcinst.h>
#include <vector>
#include<string>
//use the std namespace
//SQLWCHAR statementText2[] = L"Select * from person";
using namespace std;

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

SQLSMALLINT* count = nullptr;
SQLSMALLINT nresultcols = 0, i;


//define handles and variables
//SQLHSTMT hstmt=nullptr;
SQLRETURN   rc = 0;
SQLHANDLE sqlConnHandle = NULL;
SQLHANDLE sqlStmtHandle = NULL;
SQLHANDLE sqlEnvHandle = NULL;
SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];

struct datacheck {
	int enrollmentId;
	int courseId;
	int studentId;
	float grade;
	//constructor;
	datacheck() {};

	int server_Connect() {
		sqlConnHandle = NULL;
		sqlStmtHandle = NULL;
		//allocations
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
			return -1;
		if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			return -1;
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
			return -1;
		//outpu
		std::cout << "Attempting connection to SQL Server...";
		std::cout << "\n";
		//connect to SQL Server
		//I am using a trusted connection and port 14808
		//it does not matter if you are using default or named instance
		//just make sure you define the server name and the port
		//You have the option to use a username/password instead of a trusted connection
		//but is more secure to use a trusted connection

		switch (SQLDriverConnect(sqlConnHandle,
			NULL,
			//(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=master;UID=username;PWD=password;",
			(SQLWCHAR*)L"DRIVER={SQL Server};Server=DEV-SHAYANKHAN\\SQLEXPRESS;Database=school;Trusted_Connection=yes;",
			SQL_NTS,
			retconstring,
			1024,
			NULL,
			SQL_DRIVER_NOPROMPT)) {
		case SQL_SUCCESS:
			std::cout << "Successfully connected to SQL Server";
			std::cout << "\n";
			break;
		case SQL_SUCCESS_WITH_INFO:
			std::cout << "Successfully connected to SQL Server";
			std::cout << "\n";
			break;
		case SQL_INVALID_HANDLE:
			std::cout << "Could not connect to SQL Server " << retconstring;
			std::cout << "\n";
			return -1;
		case SQL_ERROR:
			std::cout << "Could not connect to SQL Server" << retconstring;
			std::cout << "\n";
			return -1;
		default:
			break;
		}
		//if there is a problem connecting then exit application
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
			return -1;
		//output
		std::cout << "\n";
		std::cout << "Executing SQL query...";
		std::cout << "\n";
	}

	void fetch_Data(SQLRETURN   rc, SQLHANDLE sqlConnHandle, SQLHANDLE sqlStmtHandle, SQLHANDLE sqlEnvHandle, SQLWCHAR retconstring[], SQLSMALLINT nresultcols,std::vector<datacheck> &dc) {
		//if the statement is not right show an error
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"select * from studentgrade;", SQL_NTS)) {
			//SQLRETURN retErrorCode = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"select * from groups;", SQL_NTS);
			std::cout << "Error querying SQL Server Returncode:"; //<< retErrorCode;
			std::cout << "\n";
			//return dc ;
		}
		else {
			//declare output variable and pointer0x000001e157c096f0 
			rc = SQLNumResultCols(sqlStmtHandle, &nresultcols);
			SQLCHAR dataRetrieved[SQL_RESULT_LEN]{};
			//cout << nresultcols;
			SQLLEN ptrSqlVersion;
			//for (int i = 1; i < 5; i++) {
			struct datacheck dr;
				while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
					for (int j = 1; j <= 4;j++) {
						SQLRETURN abc=SQLGetData(sqlStmtHandle, j, SQL_CHAR, dataRetrieved, SQL_RESULT_LEN, &ptrSqlVersion);
						//display query result
						//std::cout << "\n";
						//cout << dataRetrieved << endl;
						/*if (abc == -1) {
							continue;
						}*/
						if (j == 1) {
							dr.enrollmentId = std::stoi((char*)dataRetrieved);
						}
						if (j == 2) {
							dr.courseId = std::stoi((char*)dataRetrieved);
						}
						if (j == 3) {
							dr.studentId = std::stoi((char*)dataRetrieved);
						}
						if (j == 4) {
							if(ptrSqlVersion == SQL_NULL_DATA){
								dr.grade = -1;
							}
							else {
								dr.grade = std::stof((char*)dataRetrieved);
							}
								//cout << abc;
						}
						//dataRetrieved
					}
						dc.push_back(dr);
					//free(dr);
					//int snk;
				}
				std::cout << "\n";
				//SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				//SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
				//SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"select * from studentgrade", SQL_NTS);
			
			//return dc;
		}
	}

	void printdata(std::vector<datacheck>& dc) {
		for (datacheck x : dc) {
			std::cout << x.enrollmentId << " " << x.courseId << " " <<  x.studentId << " " << x.grade << std::endl;
		}
	}
};
int main() {
	std::vector<datacheck> d2;
	datacheck d1 = datacheck();
	d1.server_Connect();
	d1.fetch_Data(rc, sqlConnHandle, sqlStmtHandle, sqlEnvHandle, (SQLWCHAR *)retconstring[240],nresultcols,d2);
	d1.printdata(d2);
}