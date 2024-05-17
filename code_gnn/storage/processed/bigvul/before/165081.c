void OffscreenCanvas::Trace(blink::Visitor* visitor) {
  visitor->Trace(context_);
  visitor->Trace(execution_context_);
  EventTargetWithInlineData::Trace(visitor);
}
