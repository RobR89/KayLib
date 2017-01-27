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

#include <vector>

#include "KSQL.h"

namespace KayLib
{

    KSQL::KSQL() { }

    KSQL::~KSQL() { }

    KSQLResult::KSQLResult(int cols, const std::vector<std::vector<const KSQLCell *>> &results)
    {
        columns = cols;
        for(auto row : results)
        {
            std::vector<const KSQLCell *> nRow;
            for(auto col : row)
            {
                nRow.push_back(col);
            }
            rows.push_back(nRow);
        }
    }

    KSQLResult::~KSQLResult()
    {
        for(auto row : rows)
        {
            for(auto cel : row)
            {
                if(cel != nullptr)
                {
                    delete cel;
                }
            }
        }
    }

    std::vector<const KSQLCell *> KSQLResult::getRow(int row)
    {
        if(row < 0 || row > rows.size())
        {
            return std::vector<const KSQLCell *>();
        }
        return std::vector<const KSQLCell *>(rows[row]);
    }

    KSQLStatement::KSQLStatement() { }

    KSQLStatement::~KSQLStatement() { }

}