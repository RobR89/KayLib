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

#ifndef KSQLITE_H
#define KSQLITE_H

#include "KSQL.h"
#include <sqlite3.h>

namespace KayLib
{
    class KSQLite;

    class KSQLiteStatement : public KSQLStatement
    {
    public:
        friend KSQLite;

        KSQLiteStatement()
        {
            m_statement = nullptr;
        }

        virtual ~KSQLiteStatement()
        {
            if(m_statement != nullptr)
            {
                sqlite3_finalize(m_statement);
            }
        }

        bool bind(const int index, const char *data, const int length)
        {
            int result = sqlite3_bind_blob(m_statement, index, data, length, SQLITE_TRANSIENT);
            if(result != SQLITE_OK)
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
            static const char * const protocol = "SQLite";
            return protocol;
        }

        sqlite3_stmt *m_statement;
    };

    class KSQLite : public KSQL
    {
    public:

        KSQLite()
        {
            m_connection = nullptr;
            //TO-DO: add mutex lock to prevent multi-init.
            int &inits = getInits();
            if(inits == 0)
            {
                int ret = sqlite3_initialize();
                if(ret != SQLITE_OK)
                {
                    // Init failed.
                }
            }
            inits++;
        }

        KSQLite(const KSQLite& orig) = delete;

        virtual ~KSQLite()
        {
            if(m_connection != nullptr)
            {
                sqlite3_close(m_connection);
                m_connection = nullptr;
            }
            //TO-DO: add mutex lock to prevent premature shutdown.
            int &inits = getInits();
            inits--;
            if(inits <= 0)
            {
                sqlite3_shutdown();
            }
        }

        virtual bool connect(const std::string &host, const std::string &user, const std::string &passWord, const std::string &database)
        {
            int state = sqlite3_open(database.c_str(), &m_connection);
            if(state)
            {
                m_errorCode = sqlite3_errcode(m_connection);
                m_lastError = sqlite3_errmsg(m_connection);
                return false;
            }
            m_errorCode = 0;
            m_lastError = "";
            return true;
        }

        virtual std::shared_ptr<KSQLStatement> prepare(const std::string &query)
        {
            sqlite3_stmt *statement;
            int state = sqlite3_prepare_v2(m_connection, query.c_str(), -1, &statement, 0);
            if(state != SQLITE_OK)
            {
                m_errorCode = sqlite3_errcode(m_connection);
                m_lastError = sqlite3_errmsg(m_connection);
                return nullptr;
            }
            std::shared_ptr<KSQLiteStatement> stmt = std::make_shared<KSQLiteStatement>();
            stmt->m_statement = statement;
            return stmt;
        }

        virtual std::shared_ptr<KSQLResult> query(const std::string &query)
        {
            sqlite3_stmt *statement;
            int state = sqlite3_prepare_v2(m_connection, query.c_str(), -1, &statement, 0);
            if(state != SQLITE_OK)
            {
                m_errorCode = sqlite3_errcode(m_connection);
                m_lastError = sqlite3_errmsg(m_connection);
                return nullptr;
            }
            sqlRowList_t rows;
            int cols = sqlite3_column_count(statement);
            int result = constructResult(cols, statement, rows);
            m_affected = sqlite3_changes(m_connection);
            if(result != SQLITE_DONE)
            {
                m_errorCode = sqlite3_errcode(m_connection);
                m_lastError = sqlite3_errmsg(m_connection);
                sqlite3_finalize(statement);
                return nullptr;
            }
            std::shared_ptr<KSQLResult> res = std::make_shared<KSQLResult>(cols, rows);
            sqlite3_finalize(statement);
            m_errorCode = 0;
            m_lastError = "";
            return res;
        }

        virtual std::shared_ptr<KSQLResult> query(const std::shared_ptr<KSQLStatement> &query)
        {
            if(query.get() == nullptr || query->getProtocol() != KSQLiteStatement::protocol())
            {
                return nullptr;
            }
            KSQLiteStatement *lStatement = (KSQLiteStatement *) query.get();
            sqlRowList_t rows;
            int cols = sqlite3_column_count(lStatement->m_statement);
            int result = constructResult(cols, lStatement->m_statement, rows);
            m_affected = sqlite3_changes(m_connection);
            if(result != SQLITE_DONE)
            {
                m_errorCode = sqlite3_errcode(m_connection);
                m_lastError = sqlite3_errmsg(m_connection);
                return nullptr;
            }
            std::shared_ptr<KSQLResult> res = std::make_shared<KSQLResult>(cols, rows);
            m_errorCode = 0;
            m_lastError = "";
            return res;
        }

        virtual bool command(const std::string &query)
        {
            sqlite3_stmt *statement;
            int state = sqlite3_prepare_v2(m_connection, query.c_str(), -1, &statement, 0);
            if(state != SQLITE_OK)
            {
                m_errorCode = sqlite3_errcode(m_connection);
                m_lastError = sqlite3_errmsg(m_connection);
                return false;
            }
            int result;
            while((result = sqlite3_step(statement)) == SQLITE_ROW)
            {
                // unexpected data....
            }
            m_affected = sqlite3_changes(m_connection);
            if(result != SQLITE_DONE)
            {
                m_errorCode = sqlite3_errcode(m_connection);
                m_lastError = sqlite3_errmsg(m_connection);
                sqlite3_finalize(statement);
                return false;
            }
            sqlite3_finalize(statement);
            m_errorCode = 0;
            m_lastError = "";
            return true;
        }

        virtual bool command(const std::shared_ptr<KSQLStatement> &query)
        {
            if(query.get() == nullptr || query->getProtocol() != KSQLiteStatement::protocol())
            {
                return false;
            }
            KSQLiteStatement *lStatement = (KSQLiteStatement *) query.get();
            int result;
            while((result = sqlite3_step(lStatement->m_statement)) == SQLITE_ROW)
            {
                // unexpected data...
            }
            m_affected = sqlite3_changes(m_connection);
            if(result != SQLITE_DONE)
            {
                m_errorCode = sqlite3_errcode(m_connection);
                m_lastError = sqlite3_errmsg(m_connection);
                sqlite3_finalize(lStatement->m_statement);
                return false;
            }
            m_errorCode = 0;
            m_lastError = "";
            return true;
        }

    private:
        sqlite3 *m_connection;

        static int &getInits()
        {
            static int sqLiteInits = 0;
            return sqLiteInits;
        }

        int constructResult(const int cols, sqlite3_stmt *statement, sqlRowList_t & rows)
        {
            int result;
            while((result = sqlite3_step(statement)) == SQLITE_ROW)
            {
                sqlCellList_t values;
                for(int col = 0; col < cols; col++)
                {
                    char *data = nullptr;
                    int len = 0;
                    if(sqlite3_column_type(statement, col) == SQLITE_BLOB)
                    {
                        data = (char*) sqlite3_column_blob(statement, col);
                        len = sqlite3_column_bytes(statement, col);
                    }
                    else
                    {
                        data = (char*) sqlite3_column_text(statement, col);
                        len = std::strlen(data) + 1;
                    }
                    values.push_back(new KSQLCell(len, data));
                }
                rows.push_back(new KSQLResultRow(cols, values));
            }
            return result;
        }
    };

}

#endif /* KSQLITE_H */

