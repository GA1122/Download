void WebLocalFrameImpl::SelectWordAroundPosition(LocalFrame* frame,
                                                 VisiblePosition position) {
  TRACE_EVENT0("blink", "WebLocalFrameImpl::selectWordAroundPosition");

  frame->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  frame->Selection().SelectWordAroundPosition(position);
}
