void DOMWindow::TraceWrappers(const ScriptWrappableVisitor* visitor) const {
  visitor->TraceWrappers(location_);
  EventTargetWithInlineData::TraceWrappers(visitor);
}
