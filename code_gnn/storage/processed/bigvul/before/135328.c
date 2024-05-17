MouseEventWithHitTestResults Document::prepareMouseEvent(const HitTestRequest& request, const LayoutPoint& documentPoint, const PlatformMouseEvent& event)
{
    ASSERT(!layoutView() || layoutView()->isLayoutView());

    if (!layoutView() || !view() || !view()->didFirstLayout())
        return MouseEventWithHitTestResults(event, HitTestResult(request, LayoutPoint()));

    HitTestResult result(request, documentPoint);
    layoutView()->hitTest(result);

    if (!request.readOnly())
        updateHoverActiveState(request, result.innerElement());

    return MouseEventWithHitTestResults(event, result);
}
