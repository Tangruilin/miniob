//
// Created by Byte Dance on 2022/9/22.
//

#include "common/log/log.h"
#include "sql/operator/update_operator.h"

RC UpdateOperator::open()
{
  if (children_.size() != 1) {
    LOG_WARN("update operator must has only one children");
    return RC::INTERNAL;
  }
  
  Operator *child = children_[0];
  RC rc = child->open();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  Table *table = update_stmt_->table();
  while(RC::SUCCESS == (rc = child->next())) {
    Tuple *currentTuple = child->current_tuple();
    if (currentTuple == nullptr) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }
    
    RowTuple *currentRowTuple = static_cast<RowTuple *>(currentTuple);
    Record record = currentRowTuple->record();
    // update the record
    rc = table->update_record(nullptr, &record, update_stmt_->attributeName(), update_stmt_->values());
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to update record: %s", strrc(rc));
      return rc;
    }
  }
  return RC::SUCCESS;
}

RC UpdateOperator::next()
{
  return RC::RECORD_EOF;
}

RC UpdateOperator::close()
{
  children_[0]->close();
  return RC::SUCCESS;
}