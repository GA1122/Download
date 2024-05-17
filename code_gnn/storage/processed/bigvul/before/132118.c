GapRects LayoutBlockFlow::selectionGapRectsForPaintInvalidation(const LayoutBoxModelObject* paintInvalidationContainer) const
{
    ASSERT(!needsLayout());

    if (!shouldPaintSelectionGaps())
        return GapRects();

    TransformState transformState(TransformState::ApplyTransformDirection, FloatPoint());
    mapLocalToContainer(paintInvalidationContainer, transformState, ApplyContainerFlip | UseTransforms);
    LayoutPoint offsetFromPaintInvalidationContainer = roundedLayoutPoint(transformState.mappedPoint());

    if (hasOverflowClip())
        offsetFromPaintInvalidationContainer -= scrolledContentOffset();

    LayoutUnit lastTop = 0;
    LayoutUnit lastLeft = logicalLeftSelectionOffset(this, lastTop);
    LayoutUnit lastRight = logicalRightSelectionOffset(this, lastTop);

    return selectionGaps(this, offsetFromPaintInvalidationContainer, LayoutSize(), lastTop, lastLeft, lastRight);
}
