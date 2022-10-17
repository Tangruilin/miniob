//
// Created by Byte Dance on 2022/9/22.
//

#include "rc.h"
#include "operator.h"
#include "sql/stmt/update_stmt.h"

class UpdateOperator;

class UpdateOperator : public Operator {
public:
  UpdateOperator(UpdateStmt *update_stmt) : update_stmt_(update_stmt)
  {}

  virtual ~UpdateOperator() = default;

  RC open() override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override
  {
    return nullptr;
  }

private:
  UpdateStmt *update_stmt_ = nullptr;
};
