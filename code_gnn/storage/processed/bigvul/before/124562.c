bool RenderBlock::hitTestContents(const HitTestRequest& request, HitTestResult& result, const HitTestLocation& locationInContainer, const LayoutPoint& accumulatedOffset, HitTestAction hitTestAction)
{
    if (childrenInline() && !isTable()) {
        if (m_lineBoxes.hitTest(this, request, result, locationInContainer, accumulatedOffset, hitTestAction))
            return true;
    } else {
        HitTestAction childHitTest = hitTestAction;
        if (hitTestAction == HitTestChildBlockBackgrounds)
            childHitTest = HitTestChildBlockBackground;
        for (RenderBox* child = lastChildBox(); child; child = child->previousSiblingBox()) {
            LayoutPoint childPoint = flipForWritingModeForChild(child, accumulatedOffset);
            if (!child->hasSelfPaintingLayer() && !child->isFloating() && child->nodeAtPoint(request, result, locationInContainer, childPoint, childHitTest))
                return true;
        }
    }

    return false;
}
