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

#include <vector>
#include <memory>
#include <string>
#include "tools/converter/parser/tflite/tflite_pooling_parser.h"

namespace mindspore {
namespace lite {
STATUS TflitePoolingParser::Parse(const std::unique_ptr<tflite::OperatorT> &tflite_op,
                                  const std::vector<std::unique_ptr<tflite::TensorT>> &tfliteTensors,
                                  const std::vector<std::unique_ptr<tflite::BufferT>> &tfliteModelBuffer,
                                  const std::vector<std::unique_ptr<tflite::OperatorCodeT>> &tfliteOpSet,
                                  schema::CNodeT *op, TensorCache *tensor_cache, bool quantizedModel) {
  if (op == nullptr) {
    MS_LOG(ERROR) << "op is null";
    return RET_NULL_PTR;
  }
  op->primitive = std::make_unique<schema::PrimitiveT>();
  if (op->primitive == nullptr) {
    MS_LOG(ERROR) << "op->primitive is null";
    return RET_NULL_PTR;
  }

  std::unique_ptr<schema::PoolingT> attr(new schema::PoolingT());

  std::vector<std::string> node_name_str;
  Split(op->name.data(), &node_name_str, "-");
  const char *node_name = node_name_str.data()->c_str();
  if (std::strcmp(node_name, "MeanPooling") == 0) {
    MS_LOG(DEBUG) << "parser TfliteMeanPoolingParser";
    attr->poolingMode = schema::PoolMode_MEAN_POOLING;
  } else if (std::strcmp(node_name, "MaxPooling") == 0) {
    MS_LOG(DEBUG) << "parse TfliteMaxPoolingParser";
    attr->poolingMode = schema::PoolMode_MAX_POOLING;
  } else {
    MS_LOG(ERROR) << "wrong pooling type";
    return RET_ERROR;
  }

  const auto &tflite_attr = tflite_op->builtin_options.AsPool2DOptions();
  if (tflite_attr == nullptr) {
    MS_LOG(ERROR) << "get op: " << op->name.c_str() << " attr failed";
    return RET_NULL_PTR;
  }
  attr->windowW = tflite_attr->filter_width;
  attr->windowH = tflite_attr->filter_height;
  attr->strideW = tflite_attr->stride_w;
  attr->strideH = tflite_attr->stride_h;
  attr->padMode = GetPadMode(tflite_attr->padding);
  attr->format = schema::Format_NHWC;
  // attr->global

  // calculate pad params

  op->primitive->value.type = schema::PrimitiveType_Pooling;
  op->primitive->value.value = attr.release();
  return RET_OK;
}

TfliteNodeRegister g_tfliteMeanPoolingParser("MeanPooling", new TfliteMeanPoolingParser());
TfliteNodeRegister g_tfliteMaxPoolingParser("MaxPooling", new TfliteMaxPoolingParser());
}  // namespace lite
}  // namespace mindspore

