std::unique_ptr<TracedValue> InspectorTracingSessionIdForWorkerEvent::Data(
    const String& session_id,
    const String& worker_id,
    WorkerThread* worker_thread) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("sessionId", session_id);
  value->SetString("workerId", worker_id);
  value->SetDouble("workerThreadId", worker_thread->GetPlatformThreadId());
  return value;
}
