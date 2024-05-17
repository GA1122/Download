Document* LocalFrame::DocumentAtPoint(const LayoutPoint& point_in_root_frame) {
  if (!View())
    return nullptr;

  LayoutPoint pt = View()->ConvertFromRootFrame(point_in_root_frame);

  if (!ContentLayoutObject())
    return nullptr;
  HitTestResult result = GetEventHandler().HitTestResultAtPoint(
      pt, HitTestRequest::kReadOnly | HitTestRequest::kActive);
   return result.InnerNode() ? &result.InnerNode()->GetDocument() : nullptr;
 }
