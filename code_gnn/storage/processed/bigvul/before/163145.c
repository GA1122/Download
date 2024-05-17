void HTMLFrameOwnerElement::Trace(blink::Visitor* visitor) {
  visitor->Trace(content_frame_);
  visitor->Trace(embedded_content_view_);
  HTMLElement::Trace(visitor);
  FrameOwner::Trace(visitor);
}
