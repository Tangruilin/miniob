/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "common/log/log.h"
#include "storage/common/db.h"
#include "storage/common/table.h"

UpdateStmt::UpdateStmt(Table *table, const Value *values, int value_amount, const char *attribute_name, FilterStmt *filter_stmt)
  : table_ (table), values_(values), value_amount_(value_amount), attribute_name_(attribute_name), filter_stmt_(filter_stmt)
{}

RC UpdateStmt::create(Db *db, const Updates &update, Stmt *&stmt)
{
  const char *table_name = update.relation_name;
  // check db and table_name is not null
  if (nullptr == db || nullptr == table_name) {
    LOG_WARN("invalid argument. db=%p, table_name=%p",
        db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  
  // check the updating field exist
  const FieldMeta *updatingFieldMeta = table->table_meta().field(update.attribute_name);
  if (updatingFieldMeta == nullptr) {
    LOG_WARN("schema mismatch, updating field name=%s doesn't exist in table=%s", update.attribute_name, table_name);
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  // check field type
  AttrType fieldType = updatingFieldMeta->type();
  AttrType valueType = update.value.type;
  if (fieldType != valueType) {
    LOG_WARN("field type mismatch, updating field type=%d, new value type=%d, table=%s", fieldType, valueType, table_name);
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }

  // create filter stmt
  
  // tables
  std::unordered_map<std::string/*table_name*/, Table* /*table ptr*/> table_map;
  table_map.insert(std::pair<std::string, Table*>(std::string(table_name), table));
  
  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(db, table, &table_map, update.conditions, update.condition_num, filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }
  // right
  stmt = new UpdateStmt(table, &(update.value), 1, update.attribute_name, filter_stmt);
  return rc;
}
const char *UpdateStmt::attributeName() const
{
  return attribute_name_;
}
