void RemoteFrame::Trace(blink::Visitor* visitor) {
  visitor->Trace(view_);
  visitor->Trace(security_context_);
  Frame::Trace(visitor);
}
