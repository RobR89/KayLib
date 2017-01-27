#include "KSQLite.h"

namespace KayLib {

int sqLiteInits = 0;

KSQLite::KSQLite() {
  connection = nullptr;
  if(sqLiteInits == 0) {
    int ret = sqlite3_initialize();
    if(ret != SQLITE_OK) {
      // Init failed.
    }
  }
  sqLiteInits++;
}

KSQLite::~KSQLite() {
  if(connection != nullptr) {
    sqlite3_close(connection);
    connection = nullptr;
  }
  sqLiteInits--;
  if(sqLiteInits <= 0) {
    sqlite3_shutdown();
  }
}

bool KSQLite::connect(const std::string &host, const std::string &user, const std::string &passWord, const std::string &database) {
  int state = sqlite3_open(database.c_str(), &connection);
  if(state) {
    errorCode = sqlite3_errcode(connection);
    lastError = sqlite3_errmsg(connection);
    return false;
  }
  errorCode = 0;
  lastError = "";
  return true;
}

std::shared_ptr<KSQLStatement> KSQLite::prepare(const std::string &query) {
  sqlite3_stmt *statement;
  int state = sqlite3_prepare_v2(connection, query.c_str(), -1, &statement, 0);
  if(state != SQLITE_OK) {
    errorCode = sqlite3_errcode(connection);
    lastError = sqlite3_errmsg(connection);
    return nullptr;
  }
  KSQLiteStatement *stmt = new KSQLiteStatement();
  stmt->statement = statement;
  return std::shared_ptr<KSQLStatement>((KSQLStatement *)stmt);
}

std::shared_ptr<KSQLResult> KSQLite::query(const std::string &query) {
  sqlite3_stmt *statement;
  int state = sqlite3_prepare_v2(connection, query.c_str(), -1, &statement, 0);
  if(state != SQLITE_OK) {
    errorCode = sqlite3_errcode(connection);
    lastError = sqlite3_errmsg(connection);
    return nullptr;
  }
  std::vector<std::vector<const KSQLCell *>> rows;
  int cols = sqlite3_column_count(statement);
  int result;
  while((result = sqlite3_step(statement)) == SQLITE_ROW) {
    std::vector<const KSQLCell *> values;
    for(int col = 0; col < cols; col++) {
      char *data = nullptr;
      int len = 0;
      if(sqlite3_column_type(statement, col) == SQLITE_BLOB) {
        data = (char*)sqlite3_column_blob(statement, col);
        len = sqlite3_column_bytes(statement, col);
      } else {
        data = (char*)sqlite3_column_text(statement, col);
        len = std::strlen(data) + 1;
      }
      values.push_back(new KSQLCell(len, data));
    }
    rows.push_back(values);
  }
  affected = rows.size();
  if(result != SQLITE_DONE) {
    errorCode = sqlite3_errcode(connection);
    lastError = sqlite3_errmsg(connection);
    sqlite3_finalize(statement);
    return nullptr;
  }
  KSQLResult *res = new KSQLResult(cols, rows);
  sqlite3_finalize(statement);
  errorCode = 0;
  lastError = "";
  return std::shared_ptr<KSQLResult>(res);
}

std::shared_ptr<KSQLResult> KSQLite::query(const std::shared_ptr<KSQLStatement> &query) {
  if(query.get() == nullptr || query->getProtocol() != KSQLiteStatement::protocol) {
    return nullptr;
  }
  KSQLiteStatement *lStatement = (KSQLiteStatement *)query.get();
  std::vector<std::vector<const KSQLCell *>> rows;
  int cols = sqlite3_column_count(lStatement->statement);
  int result;
  while((result = sqlite3_step(lStatement->statement)) == SQLITE_ROW) {
    std::vector<const KSQLCell *> values;
    for(int col = 0; col < cols; col++) {
      char *data = nullptr;
      int len = 0;
      if(sqlite3_column_type((lStatement->statement), col) == SQLITE_BLOB) {
        data = (char*)sqlite3_column_blob((lStatement->statement), col);
        len = sqlite3_column_bytes((lStatement->statement), col);
      } else {
        data = (char*)sqlite3_column_text((lStatement->statement), col);
        len = std::strlen(data) + 1;
      }
      values.push_back(new KSQLCell(len, data));
    }
    rows.push_back(values);
  }
  affected = rows.size();
  if(result != SQLITE_DONE) {
    errorCode = sqlite3_errcode(connection);
    lastError = sqlite3_errmsg(connection);
    return nullptr;
  }
  KSQLResult *res = new KSQLResult(cols, rows);
  errorCode = 0;
  lastError = "";
  return std::shared_ptr<KSQLResult>(res);
}

bool KSQLite::command(const std::string &query) {
  sqlite3_stmt *statement;
  int state = sqlite3_prepare_v2(connection, query.c_str(), -1, &statement, 0);
  if(state != SQLITE_OK) {
    errorCode = sqlite3_errcode(connection);
    lastError = sqlite3_errmsg(connection);
    return false;
  }
  int result;
  affected = 0;
  while((result = sqlite3_step(statement)) == SQLITE_ROW) {
    affected++;
  }
  if(result != SQLITE_DONE) {
    errorCode = sqlite3_errcode(connection);
    lastError = sqlite3_errmsg(connection);
    sqlite3_finalize(statement);
    return false;
  }
  sqlite3_finalize(statement);
  errorCode = 0;
  lastError = "";
  return true;
}

bool KSQLite::command(const std::shared_ptr<KSQLStatement> &query) {
  if(query.get() == nullptr || query->getProtocol() != KSQLiteStatement::protocol) {
    return false;
  }
  KSQLiteStatement *lStatement = (KSQLiteStatement *)query.get();
  int result;
  affected = 0;
  while((result = sqlite3_step(lStatement->statement)) == SQLITE_ROW) {
    affected++;
  }
  if(result != SQLITE_DONE) {
    errorCode = sqlite3_errcode(connection);
    lastError = sqlite3_errmsg(connection);
    sqlite3_finalize(lStatement->statement);
    return false;
  }
  errorCode = 0;
  lastError = "";
  return true;
}

KSQLiteStatement::KSQLiteStatement() {
  statement = nullptr;
}

KSQLiteStatement::~KSQLiteStatement() {
  if(statement != nullptr) {
    sqlite3_finalize(statement);
  }
}

const char * const KSQLiteStatement::protocol = "SQLite";

bool KSQLiteStatement::bind(int index, const char *data, int length) {
  int result = sqlite3_bind_blob(statement, index, data, length, SQLITE_TRANSIENT);
  if(result != SQLITE_OK) {
    return false;
  }
  return true;
}

}
