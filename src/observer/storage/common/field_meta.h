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
// Created by Meiyi & Wangyunlai on 2021/5/12.
//

#ifndef __OBSERVER_STORAGE_COMMON_FIELD_META_H__
#define __OBSERVER_STORAGE_COMMON_FIELD_META_H__

#include <string>

#include "rc.h"
#include "sql/parser/parse_defs.h"

namespace Json {
class Value;
}  // namespace Json

// Take care of shallow copy
class FieldMeta {
public:
  FieldMeta();
  ~FieldMeta() = default;
  /**
   * Init a field with basic data
   * @param name
   * @param attr_type
   * @param attr_offset
   * @param attr_len
   * @param visible
   * @return
   */
  RC init(const char *name, AttrType attr_type, int attr_offset, int attr_len, bool visible);

public:
  const char *name() const;
  AttrType type() const;
  int offset() const;
  int len() const;
  bool visible() const;

public:
  void desc(std::ostream &os) const;

public:
  void to_json(Json::Value &json_value) const;
  /**
   * Parse table meta from json value
   * @param json_value
   * @param field
   * @return
   */
  static RC from_json(const Json::Value &json_value, FieldMeta &field);

protected:
  // field name
  std::string name_;
  // field attribute type(undefined, chars, int, floats)
  AttrType attr_type_;
  // attribute offset
  int attr_offset_;
  // attribute length
  int attr_len_;
  // visible for user
  bool visible_;
};
#endif  // __OBSERVER_STORAGE_COMMON_FIELD_META_H__