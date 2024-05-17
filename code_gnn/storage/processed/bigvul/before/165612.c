void Location::Trace(blink::Visitor* visitor) {
  visitor->Trace(dom_window_);
  ScriptWrappable::Trace(visitor);
}
