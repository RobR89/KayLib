#ifndef DBTEST_H
#define DBTEST_H

#include "DB/KSQLite.h"

using namespace KayLib;

void testSQLite()
{
    KSQL *sql = new KSQLite();
    if(sql->connect("", "", "", "KSQL-Test.sqlite") == true)
    {
        std::string qry = "drop table test";
        sql->command(qry);
        int affected = sql->rowsAffected();
        std::cout << qry << std::endl << affected << " rows affected." << std::endl;
        qry = "create table test("
                "orderID INTEGER PRIMARY KEY AUTOINCREMENT,"
                "price double not null"
                ")";
        if(sql->command(qry) == true)
        {
            affected = sql->rowsAffected();
            std::cout << qry << std::endl << affected << " rows affected." << std::endl;
            qry = "insert into test (price) values('123.456')";
            if(sql->command(qry) == true)
            {
                affected = sql->rowsAffected();
                std::cout << qry << std::endl << affected << " rows affected." << std::endl;
                qry = "insert into test (price) values ('456.789')";
                if(sql->command(qry) == true)
                {
                    affected = sql->rowsAffected();
                    std::cout << qry << std::endl << affected << " rows affected." << std::endl;
                    qry = "select * from test";
                    auto result = sql->query(qry);
                    if(result)
                    {
                        int rows = result->numRows();
                        std::cout << "returned " << rows << " rows." << std::endl;
                        for(int r = 0; r < rows; r++)
                        {
                            auto row = result->getRow(r);
                            std::cout << r << ") ";
                            int numCols = result->numColumns();
                            for(int i = 0; i < numCols; i++)
                            {
                                if(i > 0)
                                {
                                    std::cout << ", ";
                                }
                                auto col = row->getCol(i);
                                std::cout << ((!col || col->value != nullptr) ? col->value : "NULL");
                            }
                            std::cout << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "Select failed: (" << sql->getLastErrorCode() << ") " << sql->getLastError() << std::endl;
                    }
                }
                else
                {
                    std::cout << "Insert failed: (" << sql->getLastErrorCode() << ") " << sql->getLastError() << std::endl;
                }
            }
            else
            {
                std::cout << "Insert failed: (" << sql->getLastErrorCode() << ") " << sql->getLastError() << std::endl;
            }
        }
        else
        {
            std::cout << "Create table failed: (" << sql->getLastErrorCode() << ") " << sql->getLastError() << std::endl;
        }
    }
}


#endif /* DBTEST_H */

