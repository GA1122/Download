void Performance::TraceWrappers(ScriptWrappableVisitor* visitor) const {
  for (const auto& observer : observers_)
    visitor->TraceWrappers(observer);
  EventTargetWithInlineData::TraceWrappers(visitor);
}
