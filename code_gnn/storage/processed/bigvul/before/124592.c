void RenderBlock::markFixedPositionObjectForLayoutIfNeeded(RenderObject* child, SubtreeLayoutScope& layoutScope)
{
    if (child->style()->position() != FixedPosition)
        return;

    bool hasStaticBlockPosition = child->style()->hasStaticBlockPosition(isHorizontalWritingMode());
    bool hasStaticInlinePosition = child->style()->hasStaticInlinePosition(isHorizontalWritingMode());
    if (!hasStaticBlockPosition && !hasStaticInlinePosition)
        return;

    RenderObject* o = child->parent();
    while (o && !o->isRenderView() && o->style()->position() != AbsolutePosition)
        o = o->parent();
    if (o->style()->position() != AbsolutePosition)
        return;

    RenderBox* box = toRenderBox(child);
    if (hasStaticInlinePosition) {
        LogicalExtentComputedValues computedValues;
        box->computeLogicalWidth(computedValues);
        LayoutUnit newLeft = computedValues.m_position;
        if (newLeft != box->logicalLeft())
            layoutScope.setChildNeedsLayout(child);
    } else if (hasStaticBlockPosition) {
        LayoutUnit oldTop = box->logicalTop();
        box->updateLogicalHeight();
        if (box->logicalTop() != oldTop)
            layoutScope.setChildNeedsLayout(child);
    }
}
