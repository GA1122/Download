std::unique_ptr<TracedValue> InspectorHitTestEvent::EndData(
    const HitTestRequest& request,
    const HitTestLocation& location,
    const HitTestResult& result) {
  std::unique_ptr<TracedValue> value(TracedValue::Create());
  value->SetInteger("x", location.RoundedPoint().X());
  value->SetInteger("y", location.RoundedPoint().Y());
  if (location.IsRectBasedTest())
    value->SetBoolean("rect", true);
  if (location.IsRectilinear())
    value->SetBoolean("rectilinear", true);
  if (request.TouchEvent())
    value->SetBoolean("touch", true);
  if (request.Move())
    value->SetBoolean("move", true);
  if (request.ListBased())
    value->SetBoolean("listBased", true);
  else if (Node* node = result.InnerNode())
    SetNodeInfo(value.get(), node, "nodeId", "nodeName");
  return value;
}
