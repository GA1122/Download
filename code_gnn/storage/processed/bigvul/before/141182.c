MouseEventWithHitTestResults Document::PerformMouseEventHitTest(
    const HitTestRequest& request,
    const LayoutPoint& document_point,
    const WebMouseEvent& event) {
  DCHECK(!GetLayoutView() || GetLayoutView()->IsLayoutView());

  if (!GetLayoutView() || !View() || !View()->DidFirstLayout()) {
    HitTestLocation location((LayoutPoint()));
    return MouseEventWithHitTestResults(event, location,
                                        HitTestResult(request, location));
  }

  HitTestLocation location(document_point);
  HitTestResult result(request, location);
  GetLayoutView()->HitTest(location, result);

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

  return MouseEventWithHitTestResults(event, location, result);
}
