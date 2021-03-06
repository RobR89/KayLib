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

#ifndef SQL_H
#define SQL_H

#include <memory>
#include <vector>
#include <cstring>

namespace KayLib
{

    class KSQLResult;
    class KSQLStatement;
    class KSQLCell;
    class KSQLResultRow;

    typedef std::vector<const KSQLCell *> sqlCellList_t;
    typedef std::vector<const KSQLResultRow *> sqlRowList_t;

    class KSQL
    {
    public:

        KSQL() { }
        KSQL(const KSQL &) = delete;
        virtual ~KSQL() { }

        /**
         * Connect to a server.
         * @param host The host name.  For file based databases this is the file name.
         * @param user The user name.
         * @param passWord The password.
         * @param database The database.
         * @return True if the connection was successful.
         */
        virtual bool connect(const std::string &host, const std::string &user, const std::string &passWord, const std::string &database) = 0;
        /**
         * Execute a query that returns data.
         * @param query The query to be executed.
         * @return The result of the query.
         */
        virtual std::shared_ptr<KSQLStatement> prepare(const std::string &query) = 0;
        /**
         * Execute a query that returns data.
         * @param query The query to be executed.
         * @return The result of the query.
         */
        virtual std::shared_ptr<KSQLResult> query(const std::string &query) = 0;
        /**
         * Execute a query that returns data.
         * @param query The query to be executed.
         * @return The result of the query.
         */
        virtual std::shared_ptr<KSQLResult> query(const std::shared_ptr<KSQLStatement> &query) = 0;
        /**
         * Execute a query that does not return data.
         * @param query The query to be executed.
         * @return True if successful.
         */
        virtual bool command(const std::string &query) = 0;
        /**
         * Execute a query that does not return data.
         * @param query The query to be executed.
         * @return True if successful.
         */
        virtual bool command(const std::shared_ptr<KSQLStatement> &query) = 0;

        /**
         * Return the number of rows affected by the last query.
         * @return The number of rows affected.  Or -1 on error.
         */
        int rowsAffected() const
        {
            return m_affected;
        }

        /**
         * Get the last error message generated.
         * @return The error message.
         */
        std::string getLastError() const
        {
            return m_lastError;
        }

        /**
         * Get the last error code.
         * @return The error code.
         */
        int getLastErrorCode() const
        {
            return m_errorCode;
        }

    protected:
        int m_affected = 0;
        int m_errorCode;
        std::string m_lastError;

    };

    class KSQLCell
    {
    public:

        KSQLCell(const int len, const char *data) : length(len), value(KSQLCell::cpy(data, len)) { }

        KSQLCell(const KSQLCell &orig) = delete;

        virtual ~KSQLCell()
        {
            if(value != nullptr)
            {
                delete[] value;
            }
        }

        const int length;
        const char * const value;

    private:

        static const char *cpy(const char *data, const int len)
        {
            if(data == nullptr || len == 0)
            {
                return nullptr;
            }
            char *out = new char[len + 1];
            memcpy(out, data, len);
            out[len] = 0;
            return out;
        }
    };

    class KSQLResultRow
    {
    public:
        KSQLResultRow(const int cols, const sqlCellList_t &results)
        : m_cols(results)
        {
        }
        ~KSQLResultRow()
        {
            for(auto col : m_cols)
            {
                delete col;
            }
            m_cols.clear();
        }

        /**
         * Get the number of columns in each row.
         * @return The number of columns.
         */
        int numColumns() const
        {
            return m_cols.size();
        }

        const KSQLCell *operator[](const size_t index) const
        {
            if(index < 0 || index >= m_cols.size())
            {
                return nullptr;
            }
            return m_cols.at(index);
        }
        
        const KSQLCell *getCol(const size_t index) const
        {
            return (*this)[index];
        }

    private:
        sqlCellList_t m_cols;

    };
    
    class KSQLResult
    {
        friend class KSQL;
    public:

        KSQLResult(const int cols, const sqlRowList_t &results)
        : m_rows(results)
        {
            m_columns = cols;
        }

        KSQLResult(const KSQL &) = delete;

        virtual ~KSQLResult()
        {
            for(auto row : m_rows)
            {
                delete row;
            }
            m_rows.clear();
        }

        /**
         * Get the number of rows in the result.
         * @return The number of rows.
         */
        virtual int numRows() const
        {
            return m_rows.size();
        }

        /**
         * Get the number of columns in each row.
         * @return The number of columns.
         */
        virtual int numColumns() const
        {
            return m_columns;
        }

        /**
         * Get the a result row.
         * @param row The row to get.
         * @return The result row.
         */
        virtual const KSQLResultRow *getRow(int row) const
        {
            if(row < 0 || row >= m_rows.size())
            {
                return nullptr;
            }
            return m_rows[row];
        }

    private:
        int m_columns;
        sqlRowList_t m_rows;

    };

    class KSQLStatement
    {
    public:
        friend KSQL;
    public:

        KSQLStatement() { }

        virtual ~KSQLStatement() { }

        /**
         * Bind data to a statement index
         * @param index The index to bind to.
         * @param data The data to bind.
         * @param length The length of the data.
         * @return True on success.
         */
        virtual bool bind(const int index, const char *data, const int length) = 0;

        /**
         * Get an identifier string that indicates what type of prepared statement this is.
         * @return The identifier string.
         * @note Implementing classes should use a static char * member and compare the
         * returned pointer not the actual string.
         */
        virtual const char *getProtocol() const = 0;

    };

}

#endif /* SQL_H */

