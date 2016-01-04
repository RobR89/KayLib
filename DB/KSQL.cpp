#include <vector>

#include "KSQL.h"

namespace KayLib {

KSQL::KSQL() {
}

KSQL::~KSQL() {
}

KSQLResult::KSQLResult(int cols, const std::vector<std::vector<const KSQLCell *>> &results) {
  columns = cols;
  for(auto row : results) {
    std::vector<const KSQLCell *> nRow;
    for(auto col : row) {
      nRow.push_back(col);
    }
    rows.push_back(nRow);
  }
}

KSQLResult::~KSQLResult() {
  for(auto row : rows) {
    for(auto cel : row) {
      if(cel != nullptr) {
        delete cel;
      }
    }
  }
}

std::vector<const KSQLCell *> KSQLResult::getRow(int row) {
  if(row < 0 || row > rows.size()) {
    return std::vector<const KSQLCell *>();
  }
  return std::vector<const KSQLCell *>(rows[row]);
}

KSQLStatement::KSQLStatement() {
}

KSQLStatement::~KSQLStatement() {
}

}