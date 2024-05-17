bool RenderBox::hasUnsplittableScrollingOverflow() const
{
    bool isHorizontal = isHorizontalWritingMode();
    if ((isHorizontal && !scrollsOverflowY()) || (!isHorizontal && !scrollsOverflowX()))
        return false;

    return !style()->logicalHeight().isIntrinsicOrAuto()
        || (!style()->logicalMaxHeight().isIntrinsicOrAuto() && !style()->logicalMaxHeight().isUndefined() && (!style()->logicalMaxHeight().isPercent() || percentageLogicalHeightIsResolvable(this)))
        || (!style()->logicalMinHeight().isIntrinsicOrAuto() && style()->logicalMinHeight().isPositive() && (!style()->logicalMinHeight().isPercent() || percentageLogicalHeightIsResolvable(this)));
}
