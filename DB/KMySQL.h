/*
 * Copyright 2017 Robert Reinhart.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MYSQL_H
#define MYSQL_H

#include "KSQL.h"
#include <mysql/mysql.h>

namespace KayLib
{

    class KMySQL;

    class KMySQLStatement : public KSQLStatement
    {
    public:
        friend KMySQL;
    private:

        KMySQLStatement()
        {
            statement = nullptr;
        }

        virtual ~KMySQLStatement()
        {
            if(statement != nullptr)
            {
                mysql_stmt_close(statement);
                statement = nullptr;
            }
        }

        bool bind(int index, const char *data, int length)
        {
            MYSQL_BIND bnd;
            memset(&bnd, 0, sizeof (bnd));
            bnd.buffer_type = enum_field_types::MYSQL_TYPE_BLOB;
            int result = mysql_stmt_bind_param(statement, &bnd);
            if(result != 0)
            {
                return false;
            }
            return true;
        }

        const char *getProtocol() const
        {
            return protocol();
        }

    protected:

        static const char * const protocol()
        {
            static const char * const protocol = "MySQL";
            return protocol;
        }
        MYSQL_STMT *statement;
    };

    class KMySQL : public KSQL
    {
    public:

        KMySQL()
        {
            connection = nullptr;
            mysql_init(&mysql);
        }

        KMySQL(const KMySQL& orig) { }

        virtual ~KMySQL()
        {
            if(connection != nullptr)
            {
                mysql_close(connection);
                connection = nullptr;
            }
        }

        virtual bool connect(const std::string &host, const std::string &user, const std::string &passWord, const std::string &database)
        {
            connection = mysql_real_connect(&mysql, host.c_str(), user.c_str(), passWord.c_str(), database.c_str(), 0, 0, 0);
            if(connection == nullptr)
            {
                errorCode = mysql_errno(&mysql);
                lastError = mysql_error(&mysql);
                return false;
            }
            errorCode = 0;
            lastError = "";
            return true;
        }

        virtual std::shared_ptr<KSQLStatement> prepare(const std::string &query)
        {
            MYSQL_STMT *statement = mysql_stmt_init(connection);
            if(statement == nullptr)
            {
                return nullptr;
            }
            int state = mysql_stmt_prepare(statement, query.c_str(), query.length());
            if(state != 0)
            {
                errorCode = mysql_errno(connection);
                lastError = mysql_error(connection);
                return nullptr;
            }
            KMySQLStatement *stmt = new KMySQLStatement();
            stmt->statement = statement;
            return std::shared_ptr<KSQLStatement>((KSQLStatement *) stmt);
        }

        virtual std::shared_ptr<KSQLResult> query(const std::string &query)
        {
            int state = mysql_query(connection, query.c_str());
            if(state != 0)
            {
                errorCode = mysql_errno(connection);
                lastError = mysql_error(connection);
                return nullptr;
            }
            MYSQL_RES *result = mysql_store_result(connection);
            if(result == nullptr)
            {
                errorCode = mysql_errno(&mysql);
                lastError = mysql_error(&mysql);
                return nullptr;
            }
            int nRows = 0;
            int nCols = mysql_num_fields(result);
            std::vector<std::vector<const KSQLCell *>> rows;
            char **raw;
            while((raw = mysql_fetch_row(result)) != nullptr)
            {
                unsigned long* len = mysql_fetch_lengths(result);
                nRows++;
                std::vector<const KSQLCell *> row;
                for(int c = 0; c < nCols; c++)
                {
                    row.push_back(new KSQLCell(len[c], raw[c]));
                }
                rows.push_back(row);
            }
            affected = mysql_affected_rows(connection);
            std::shared_ptr<KSQLResult> res = std::make_shared<KSQLResult>(nCols, rows);
            errorCode = 0;
            lastError = "";
            mysql_free_result(result);
            return res;
        }

        virtual std::shared_ptr<KSQLResult> query(const std::shared_ptr<KSQLStatement> &query)
        {
            if(!query || query->getProtocol() != KMySQLStatement::protocol())
            {
                return nullptr;
            }
            KMySQLStatement *lStatement = (KMySQLStatement *) query.get();
            int state = mysql_stmt_execute(lStatement->statement);
            if(state != 0)
            {
                errorCode = mysql_errno(connection);
                lastError = mysql_error(connection);
                return nullptr;
            }
            mysql_stmt_fetch(lStatement->statement);
            MYSQL_RES *result = mysql_store_result(connection);
            if(result == nullptr)
            {
                errorCode = mysql_errno(&mysql);
                lastError = mysql_error(&mysql);
                return nullptr;
            }
            int nRows = 0;
            int nCols = mysql_num_fields(result);
            std::vector<std::vector<const KSQLCell *>> rows;
            char **raw;
            while((raw = mysql_fetch_row(result)) != nullptr)
            {
                unsigned long* len = mysql_fetch_lengths(result);
                nRows++;
                std::vector<const KSQLCell *> row;
                for(int c = 0; c < nCols; c++)
                {
                    row.push_back(new KSQLCell(len[c], raw[c]));
                }
                rows.push_back(row);
            }
            affected = mysql_affected_rows(connection);
            std::shared_ptr<KSQLResult> res = std::make_shared<KSQLResult>(nCols, rows);
            errorCode = 0;
            lastError = "";
            mysql_free_result(result);
            return res;
        }

        virtual bool command(const std::string &query)
        {
            int state = mysql_query(connection, query.c_str());
            if(state != 0)
            {
                errorCode = mysql_errno(connection);
                lastError = mysql_error(connection);
                return false;
            }
            MYSQL_RES *result = mysql_store_result(connection);
            if(result)
            {
                mysql_free_result(result);
                //return Warning("Command returned unexpected data.");
                return false;
            }
            else
            {
                errorCode = mysql_errno(connection);
                if(errorCode == 0)
                {
                    lastError = "";
                    return true;
                }
                lastError = mysql_error(connection);
                return false;
            }
            affected = mysql_affected_rows(connection);
            errorCode = 0;
            lastError = "";
            return true;
        }

        virtual bool command(const std::shared_ptr<KSQLStatement> &query)
        {
            if(!query || query->getProtocol() != KMySQLStatement::protocol())
            {
                return false;
            }
            KMySQLStatement *lStatement = (KMySQLStatement *) query.get();
            int state = mysql_stmt_execute(lStatement->statement);
            if(state != 0)
            {
                errorCode = mysql_errno(connection);
                lastError = mysql_error(connection);
                return false;
            }
            mysql_stmt_fetch(lStatement->statement);
            MYSQL_RES *result = mysql_store_result(connection);
            if(result)
            {
                mysql_free_result(result);
                //return Warning("Command returned unexpected data.");
                return false;
            }
            else
            {
                errorCode = mysql_errno(connection);
                if(errorCode == 0)
                {
                    lastError = "";
                    return true;
                }
                lastError = mysql_error(connection);
                return false;
            }
            affected = mysql_affected_rows(connection);
            errorCode = 0;
            lastError = "";
            return true;
        }

    private:
        MYSQL *connection;
        MYSQL mysql;
    };

}

#endif /* MYSQL_H */

