/*
 * Copyright 2016 Robert Reinhart.
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
#define	KSQLITE_H

#include "KSQL.h"
#include <sqlite3.h>

namespace KayLib {

class KSQLite : public KSQL {
public:
  KSQLite();
  KSQLite(const KSQLite& orig) = delete;
  virtual ~KSQLite();

  virtual bool connect(const std::string &host, const std::string &user, const std::string &passWord, const std::string &database);
  virtual std::shared_ptr<KSQLStatement> prepare(const std::string &query);
  virtual std::shared_ptr<KSQLResult> query(const std::string &query);
  virtual std::shared_ptr<KSQLResult> query(const std::shared_ptr<KSQLStatement> &query);
  virtual bool command(const std::string &query);
  virtual bool command(const std::shared_ptr<KSQLStatement> &query);

private:
  sqlite3 *connection;
};

class KSQLiteStatement {
public:
  friend KSQLite;
private:
  KSQLiteStatement();
  virtual ~KSQLiteStatement();

  bool bind(int index, const char *data, int length);

  const char *getProtocol() const {
    return protocol;
  }

protected:

  static const char * const protocol;

  sqlite3_stmt *statement;
};

}

#endif	/* KSQLITE_H */

