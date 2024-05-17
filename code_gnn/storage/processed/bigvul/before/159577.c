MouseEventWithHitTestResults Document::PerformMouseEventHitTest(
    const HitTestRequest& request,
    const LayoutPoint& document_point,
    const WebMouseEvent& event) {
  DCHECK(GetLayoutViewItem().IsNull() || GetLayoutViewItem().IsLayoutView());

  if (GetLayoutViewItem().IsNull() || !View() || !View()->DidFirstLayout())
    return MouseEventWithHitTestResults(event,
                                        HitTestResult(request, LayoutPoint()));

  HitTestResult result(request, document_point);
  GetLayoutViewItem().HitTest(result);

  if (!request.ReadOnly())
    UpdateHoverActiveState(request, result.InnerElement());

  if (auto* canvas = ToHTMLCanvasElementOrNull(result.InnerNode())) {
    HitTestCanvasResult* hit_test_canvas_result =
        canvas->GetControlAndIdIfHitRegionExists(
            result.PointInInnerNodeFrame());
    if (hit_test_canvas_result->GetControl()) {
      result.SetInnerNode(hit_test_canvas_result->GetControl());
    }
    result.SetCanvasRegionId(hit_test_canvas_result->GetId());
  }

  return MouseEventWithHitTestResults(event, result);
}
