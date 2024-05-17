void WebLocalFrameImpl::CopyImageAt(const WebPoint& pos_in_viewport) {
  HitTestResult result = HitTestResultForVisualViewportPos(pos_in_viewport);
  if (!isHTMLCanvasElement(result.InnerNodeOrImageMapImage()) &&
      result.AbsoluteImageURL().IsEmpty()) {
    return;
  }

  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  GetFrame()->GetEditor().CopyImage(result);
}
