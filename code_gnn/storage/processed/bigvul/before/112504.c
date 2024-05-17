PassRefPtr<NodeList> Document::nodesFromRect(int centerX, int centerY, unsigned topPadding, unsigned rightPadding, unsigned bottomPadding, unsigned leftPadding, HitTestRequest::HitTestRequestType hitType) const
{
    if (!renderer())
        return 0;
    Frame* frame = this->frame();
    if (!frame)
        return 0;
    FrameView* frameView = frame->view();
    if (!frameView)
        return 0;

    float zoomFactor = frame->pageZoomFactor();
    LayoutPoint point = roundedLayoutPoint(FloatPoint(centerX * zoomFactor + view()->scrollX(), centerY * zoomFactor + view()->scrollY()));

    HitTestRequest request(hitType);

    if (!request.ignoreClipping() && !frameView->visibleContentRect().intersects(HitTestLocation::rectForPoint(point, topPadding, rightPadding, bottomPadding, leftPadding)))
        return 0;

    if (!topPadding && !rightPadding && !bottomPadding && !leftPadding) {
        HitTestResult result(point);
        return handleZeroPadding(request, result);
    }

    HitTestResult result(point, topPadding, rightPadding, bottomPadding, leftPadding);
    renderView()->hitTest(request, result);

    return StaticHashSetNodeList::adopt(result.rectBasedTestResult());
}
