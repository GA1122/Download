std::unique_ptr<TracedValue> InspectorUpdateCountersEvent::Data() {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  if (IsMainThread()) {
    value->SetInteger("documents", InstanceCounters::CounterValue(
                                       InstanceCounters::kDocumentCounter));
    value->SetInteger("nodes", InstanceCounters::CounterValue(
                                   InstanceCounters::kNodeCounter));
    value->SetInteger("jsEventListeners",
                      InstanceCounters::CounterValue(
                          InstanceCounters::kJSEventListenerCounter));
  }
  value->SetDouble("jsHeapSizeUsed", static_cast<double>(UsedHeapSize()));
  return value;
}
