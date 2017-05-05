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
            m_statement = nullptr;
        }

        virtual ~KMySQLStatement()
        {
            if(m_statement != nullptr)
            {
                mysql_stmt_close(m_statement);
                m_statement = nullptr;
            }
        }

        bool bind(const int index, const char *data, const int length)
        {
            MYSQL_BIND bnd;
            memset(&bnd, 0, sizeof (bnd));
            bnd.buffer_type = enum_field_types::MYSQL_TYPE_BLOB;
            int result = mysql_stmt_bind_param(m_statement, &bnd);
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
        MYSQL_STMT *m_statement;
    };

    class KMySQL : public KSQL
    {
    public:

        KMySQL()
        {
            m_connection = nullptr;
            mysql_init(&m_mysql);
        }

        KMySQL(const KMySQL& orig) { }

        virtual ~KMySQL()
        {
            if(m_connection != nullptr)
            {
                mysql_close(m_connection);
                m_connection = nullptr;
            }
        }

        virtual bool connect(const std::string &host, const std::string &user, const std::string &passWord, const std::string &database)
        {
            m_connection = mysql_real_connect(&m_mysql, host.c_str(), user.c_str(), passWord.c_str(), database.c_str(), 0, 0, 0);
            if(m_connection == nullptr)
            {
                m_errorCode = mysql_errno(&m_mysql);
                m_lastError = mysql_error(&m_mysql);
                return false;
            }
            m_errorCode = 0;
            m_lastError = "";
            return true;
        }

        virtual std::shared_ptr<KSQLStatement> prepare(const std::string &query)
        {
            MYSQL_STMT *statement = mysql_stmt_init(m_connection);
            if(statement == nullptr)
            {
                return nullptr;
            }
            int state = mysql_stmt_prepare(statement, query.c_str(), query.length());
            if(state != 0)
            {
                m_errorCode = mysql_errno(m_connection);
                m_lastError = mysql_error(m_connection);
                return nullptr;
            }
            KMySQLStatement *stmt = new KMySQLStatement();
            stmt->m_statement = statement;
            return std::shared_ptr<KSQLStatement>((KSQLStatement *) stmt);
        }

        virtual std::shared_ptr<KSQLResult> query(const std::string &query)
        {
            int state = mysql_query(m_connection, query.c_str());
            if(state != 0)
            {
                m_errorCode = mysql_errno(m_connection);
                m_lastError = mysql_error(m_connection);
                return nullptr;
            }
            MYSQL_RES *result = mysql_store_result(m_connection);
            if(result == nullptr)
            {
                m_errorCode = mysql_errno(&m_mysql);
                m_lastError = mysql_error(&m_mysql);
                return nullptr;
            }
            int cols = mysql_num_fields(result);
            sqlRowList_t rows;
            constructResult(cols, result, rows);
            m_affected = mysql_affected_rows(m_connection);
            std::shared_ptr<KSQLResult> res = std::make_shared<KSQLResult>(cols, rows);
            m_errorCode = 0;
            m_lastError = "";
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
            int state = mysql_stmt_execute(lStatement->m_statement);
            if(state != 0)
            {
                m_errorCode = mysql_errno(m_connection);
                m_lastError = mysql_error(m_connection);
                return nullptr;
            }
            mysql_stmt_fetch(lStatement->m_statement);
            MYSQL_RES *result = mysql_store_result(m_connection);
            if(result == nullptr)
            {
                m_errorCode = mysql_errno(&m_mysql);
                m_lastError = mysql_error(&m_mysql);
                return nullptr;
            }
            int cols = mysql_num_fields(result);
            sqlRowList_t rows;
            constructResult(cols, result, rows);
            m_affected = mysql_affected_rows(m_connection);
            std::shared_ptr<KSQLResult> res = std::make_shared<KSQLResult>(cols, rows);
            m_errorCode = 0;
            m_lastError = "";
            mysql_free_result(result);
            return res;
        }

        virtual bool command(const std::string &query)
        {
            int state = mysql_query(m_connection, query.c_str());
            if(state != 0)
            {
                m_errorCode = mysql_errno(m_connection);
                m_lastError = mysql_error(m_connection);
                return false;
            }
            MYSQL_RES *result = mysql_store_result(m_connection);
            if(result)
            {
                mysql_free_result(result);
                //return Warning("Command returned unexpected data.");
                return false;
            }
            else
            {
                m_errorCode = mysql_errno(m_connection);
                if(m_errorCode == 0)
                {
                    m_lastError = "";
                    return true;
                }
                m_lastError = mysql_error(m_connection);
                return false;
            }
            m_affected = mysql_affected_rows(m_connection);
            m_errorCode = 0;
            m_lastError = "";
            return true;
        }

        virtual bool command(const std::shared_ptr<KSQLStatement> &query)
        {
            if(!query || query->getProtocol() != KMySQLStatement::protocol())
            {
                return false;
            }
            KMySQLStatement *lStatement = (KMySQLStatement *) query.get();
            int state = mysql_stmt_execute(lStatement->m_statement);
            if(state != 0)
            {
                m_errorCode = mysql_errno(m_connection);
                m_lastError = mysql_error(m_connection);
                return false;
            }
            mysql_stmt_fetch(lStatement->m_statement);
            MYSQL_RES *result = mysql_store_result(m_connection);
            if(result)
            {
                mysql_free_result(result);
                //return Warning("Command returned unexpected data.");
                return false;
            }
            else
            {
                m_errorCode = mysql_errno(m_connection);
                if(m_errorCode == 0)
                {
                    m_lastError = "";
                    return true;
                }
                m_lastError = mysql_error(m_connection);
                return false;
            }
            m_affected = mysql_affected_rows(m_connection);
            m_errorCode = 0;
            m_lastError = "";
            return true;
        }

    private:
        MYSQL *m_connection;
        MYSQL m_mysql;
        void constructResult(const int cols, MYSQL_RES *result, sqlRowList_t & rows)
        {
            char **raw;
            while((raw = mysql_fetch_row(result)) != nullptr)
            {
                unsigned long* len = mysql_fetch_lengths(result);
                sqlCellList_t row;
                for(int c = 0; c < cols; c++)
                {
                    row.push_back(new KSQLCell(len[c], raw[c]));
                }
                rows.push_back(new KSQLResultRow(cols, row));
            }
        }
    };

}

#endif /* MYSQL_H */

