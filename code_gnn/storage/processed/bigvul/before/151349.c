std::unique_ptr<TracedValue> FillCommonPart(ContainerNode& node,
                                            const char* reason) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(node.GetDocument().GetFrame()));
  SetNodeInfo(value.get(), &node, "nodeId", "nodeName");
  value->SetString("reason", reason);
  return value;
}
