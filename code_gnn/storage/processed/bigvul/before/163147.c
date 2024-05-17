Document* HTMLFrameOwnerElement::contentDocument() const {
  return (content_frame_ && content_frame_->IsLocalFrame())
             ? ToLocalFrame(content_frame_)->GetDocument()
             : nullptr;
}
