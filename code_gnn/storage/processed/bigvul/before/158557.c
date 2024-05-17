void WebLocalFrameImpl::MoveCaretSelection(const WebPoint& point_in_viewport) {
  TRACE_EVENT0("blink", "WebLocalFrameImpl::moveCaretSelection");

  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  const IntPoint point_in_contents =
      GetFrame()->View()->ViewportToFrame(point_in_viewport);
  GetFrame()->Selection().MoveCaretSelection(point_in_contents);
}
