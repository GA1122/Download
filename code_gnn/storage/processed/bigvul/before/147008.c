HitTestResult WebLocalFrameImpl::HitTestResultForVisualViewportPos(
    const IntPoint& pos_in_viewport) {
  IntPoint root_frame_point(
      GetFrame()->GetPage()->GetVisualViewport().ViewportToRootFrame(
          pos_in_viewport));
  IntPoint doc_point(GetFrame()->View()->RootFrameToContents(root_frame_point));
  HitTestResult result = GetFrame()->GetEventHandler().HitTestResultAtPoint(
      doc_point, HitTestRequest::kReadOnly | HitTestRequest::kActive);
  result.SetToShadowHostIfInRestrictedShadowRoot();
  return result;
}
