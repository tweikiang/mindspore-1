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

#include "tools/converter/parser/onnx/onnx_batchnorm_parser.h"
#include <memory>

namespace mindspore {
namespace lite {
STATUS OnnxBatchNormParser::Parse(const onnx::GraphProto &onnx_graph,
                                  const onnx::NodeProto &onnx_node,
                                  schema::CNodeT *op) {
  MS_LOG(DEBUG) << "onnx BatchNormParser";
  if (op == nullptr) {
    MS_LOG(ERROR) << "op is null";
    return RET_NULL_PTR;
  }
  op->primitive = std::make_unique<schema::PrimitiveT>();
  if (op->primitive == nullptr) {
    MS_LOG(ERROR) << "op->primitive is null";
    return RET_NULL_PTR;
  }

  std::unique_ptr<schema::FusedBatchNormT> attr = std::make_unique<schema::FusedBatchNormT>();
  if (attr == nullptr) {
    MS_LOG(ERROR) << "new op failed";
    return RET_NULL_PTR;
  }

  for (const auto &onnx_node_attr : onnx_node.attribute()) {
    if (onnx_node_attr.name() == "epsilon") {
      attr->epsilon = onnx_node_attr.f();
    } else if (onnx_node_attr.name() == "momentum") {
      attr->momentum = onnx_node_attr.f();
    } else if (onnx_node_attr.name() == "spatial") {
      attr->spatial = static_cast<int32_t>(onnx_node_attr.i());
    }
  }

  op->primitive->value.type = schema::PrimitiveType_FusedBatchNorm;
  op->primitive->value.value = attr.release();
  return RET_OK;
}

OnnxNodeRegistrar g_onnxBatchNormParser("BatchNormalization", new OnnxBatchNormParser());
}  // namespace lite
}  // namespace mindspore
