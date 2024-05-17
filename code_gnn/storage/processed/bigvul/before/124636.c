GapRects RenderBlock::selectionGapRectsForRepaint(const RenderLayerModelObject* repaintContainer)
{
    ASSERT(!needsLayout());

    if (!shouldPaintSelectionGaps())
        return GapRects();

    TransformState transformState(TransformState::ApplyTransformDirection, FloatPoint());
    mapLocalToContainer(repaintContainer, transformState, ApplyContainerFlip | UseTransforms);
    LayoutPoint offsetFromRepaintContainer = roundedLayoutPoint(transformState.mappedPoint());

    if (hasOverflowClip())
        offsetFromRepaintContainer -= scrolledContentOffset();

    LayoutUnit lastTop = 0;
    LayoutUnit lastLeft = logicalLeftSelectionOffset(this, lastTop);
    LayoutUnit lastRight = logicalRightSelectionOffset(this, lastTop);

    return selectionGaps(this, offsetFromRepaintContainer, IntSize(), lastTop, lastLeft, lastRight);
}
