void WebContentsImpl::ClearFocusedElement() {
  if (auto* frame = GetFocusedFrame())
    frame->ClearFocusedElement();
}
