std::unique_ptr<TracedValue> InspectorAsyncTask::Data(const String& name) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("name", name);
  return value;
}
