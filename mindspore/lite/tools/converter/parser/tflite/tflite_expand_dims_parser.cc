/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tools/converter/parser/tflite/tflite_expand_dims_parser.h"
#include <vector>
#include <memory>
#include <map>

namespace mindspore {
namespace lite {
STATUS TfliteExpandDimsParser::Parse(const std::unique_ptr<tflite::OperatorT> &tflite_op,
                                     const std::vector<std::unique_ptr<tflite::TensorT>> &tflite_tensors,
                                     const std::vector<std::unique_ptr<tflite::BufferT>> &tflite_model_buffer,
                                     schema::CNodeT *op,
                                     std::vector<int32_t> *tensors_id,
                                     std::vector<schema::Format> *tensors_format,
                                     std::map<int, int>  *tensors_id_map) {
  MS_LOG(DEBUG) << "parse TfliteExpandDimsParser";

  if (op == nullptr) {
    MS_LOG(ERROR) << "op is null";
    return RET_NULL_PTR;
  }
  op->primitive = std::make_unique<schema::PrimitiveT>();
  if (op->primitive == nullptr) {
    MS_LOG(ERROR) << "op->primitive is null";
    return RET_NULL_PTR;
  }

  std::unique_ptr<schema::ExpandDimsT> attr = std::make_unique<schema::ExpandDimsT>();

  const auto &tflite_attr = tflite_op->builtin_options.AsExpandDimsOptions();
  if (tflite_attr == nullptr) {
    MS_LOG(ERROR) << "get op: " << op->name.c_str() << " attr failed";
    return RET_NULL_PTR;
  }

  attr->dim = -1;

  MS_LOG(ERROR) << "The attr dim is folded by TFLite.";
  return RET_ERROR;

  /*
  if (op != nullptr) {
    op->primitive = std::make_unique<schema::PrimitiveT>();
    op->primitive->value.type = schema::PrimitiveType_ExpandDims;
    op->primitive->value.value = attr.release();
  }
  return RET_OK;
   */
}

TfliteNodeRegister g_tfliteExpandDimsParser("ExpandDims", new TfliteExpandDimsParser());
}  // namespace lite
}  // namespace mindspore


