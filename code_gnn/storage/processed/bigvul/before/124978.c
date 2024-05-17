void RenderFlexibleBox::adjustAlignmentForChild(RenderBox* child, LayoutUnit delta)
{
    if (child->isOutOfFlowPositioned()) {
        LayoutUnit staticInlinePosition = child->layer()->staticInlinePosition();
        LayoutUnit staticBlockPosition = child->layer()->staticBlockPosition();
        LayoutUnit mainAxis = isColumnFlow() ? staticBlockPosition : staticInlinePosition;
        LayoutUnit crossAxis = isColumnFlow() ? staticInlinePosition : staticBlockPosition;
        crossAxis += delta;
        prepareChildForPositionedLayout(child, mainAxis, crossAxis, NoFlipForRowReverse);
        return;
    }

    setFlowAwareLocationForChild(child, flowAwareLocationForChild(child) + LayoutSize(0, delta));
}
