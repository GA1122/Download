std::unique_ptr<TracedValue> InspectorUpdateLayerTreeEvent::Data(
    LocalFrame* frame) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(frame));
  return value;
}
