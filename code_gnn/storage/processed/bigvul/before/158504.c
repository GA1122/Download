void WebLocalFrameImpl::CheckCompleted() {
  GetFrame()->GetDocument()->CheckCompleted();
}
