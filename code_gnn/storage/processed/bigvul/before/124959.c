bool RenderBox::sizesLogicalWidthToFitContent(const Length& logicalWidth) const
{
    if (isFloating() || (isInlineBlockOrInlineTable() && !isMarquee()))
        return true;

    if (logicalWidth.type() == Intrinsic)
        return true;

    if (parent()->isMarquee()) {
        EMarqueeDirection dir = parent()->style()->marqueeDirection();
        if (dir == MAUTO || dir == MFORWARD || dir == MBACKWARD || dir == MLEFT || dir == MRIGHT)
            return true;
    }

    if (parent()->isFlexibleBox()) {
        if (!parent()->style()->isColumnFlexDirection() || parent()->style()->flexWrap() != FlexNoWrap)
            return true;
        if (!columnFlexItemHasStretchAlignment(this))
            return true;
    }

    if (parent()->isDeprecatedFlexibleBox() && (parent()->style()->boxOrient() == HORIZONTAL || parent()->style()->boxAlign() != BSTRETCH))
        return true;

    if (logicalWidth.isAuto() && !isStretchingColumnFlexItem(this) && autoWidthShouldFitContent())
        return true;

    if (isHorizontalWritingMode() != containingBlock()->isHorizontalWritingMode())
        return true;

    return false;
}
