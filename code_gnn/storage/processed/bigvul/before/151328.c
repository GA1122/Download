std::unique_ptr<TracedValue> InspectorEventDispatchEvent::Data(
    const Event& event) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("type", event.type());
  SetCallStack(value.get());
  return value;
}
