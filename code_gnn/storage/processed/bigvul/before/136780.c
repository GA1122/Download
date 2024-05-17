void LocalDOMWindow::TraceWrappers(
    const ScriptWrappableVisitor* visitor) const {
  visitor->TraceWrappers(custom_elements_);
  visitor->TraceWrappers(document_);
  visitor->TraceWrappers(modulator_);
  visitor->TraceWrappers(navigator_);
  DOMWindow::TraceWrappers(visitor);
  Supplementable<LocalDOMWindow>::TraceWrappers(visitor);
}
