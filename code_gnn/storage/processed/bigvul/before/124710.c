void RenderBlockFlow::determineLogicalLeftPositionForChild(RenderBox* child, ApplyLayoutDeltaMode applyDelta)
{
    LayoutUnit startPosition = borderStart() + paddingStart();
    if (style()->shouldPlaceBlockDirectionScrollbarOnLogicalLeft())
        startPosition -= verticalScrollbarWidth();
    LayoutUnit totalAvailableLogicalWidth = borderAndPaddingLogicalWidth() + availableLogicalWidth();

    LayoutUnit childMarginStart = marginStartForChild(child);
    LayoutUnit newPosition = startPosition + childMarginStart;

    if (child->avoidsFloats() && containsFloats() && !flowThreadContainingBlock())
        newPosition += computeStartPositionDeltaForChildAvoidingFloats(child, marginStartForChild(child));

    setLogicalLeftForChild(child, style()->isLeftToRightDirection() ? newPosition : totalAvailableLogicalWidth - newPosition - logicalWidthForChild(child), applyDelta);
}
