bool WebLocalFrameImpl::SelectWordAroundCaret() {
  TRACE_EVENT0("blink", "WebLocalFrameImpl::selectWordAroundCaret");

  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  return GetFrame()->Selection().SelectWordAroundCaret();
}
