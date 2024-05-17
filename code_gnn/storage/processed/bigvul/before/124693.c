LayoutUnit RenderBlockFlow::computeStartPositionDeltaForChildAvoidingFloats(const RenderBox* child, LayoutUnit childMarginStart)
{
    LayoutUnit startPosition = startOffsetForContent();

    LayoutUnit oldPosition = startPosition + childMarginStart;
    LayoutUnit newPosition = oldPosition;

    LayoutUnit blockOffset = logicalTopForChild(child);
    LayoutUnit startOff = startOffsetForLine(blockOffset, false, logicalHeightForChild(child));

    if (style()->textAlign() != WEBKIT_CENTER && !child->style()->marginStartUsing(style()).isAuto()) {
        if (childMarginStart < 0)
            startOff += childMarginStart;
        newPosition = max(newPosition, startOff);  
    } else if (startOff != startPosition) {
        newPosition = startOff + childMarginStart;
    }

    return newPosition - oldPosition;
}
