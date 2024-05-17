LayoutUnit RenderBlock::adjustForUnsplittableChild(RenderBox* child, LayoutUnit logicalOffset, bool includeMargins)
{
    bool checkColumnBreaks = view()->layoutState()->isPaginatingColumns() || flowThreadContainingBlock();
    bool checkPageBreaks = !checkColumnBreaks && view()->layoutState()->pageLogicalHeight();
    bool isUnsplittable = child->isUnsplittableForPagination() || (checkColumnBreaks && child->style()->columnBreakInside() == PBAVOID)
        || (checkPageBreaks && child->style()->pageBreakInside() == PBAVOID);
    if (!isUnsplittable)
        return logicalOffset;
    LayoutUnit childLogicalHeight = logicalHeightForChild(child) + (includeMargins ? marginBeforeForChild(child) + marginAfterForChild(child) : LayoutUnit());
    LayoutUnit pageLogicalHeight = pageLogicalHeightForOffset(logicalOffset);
    updateMinimumPageHeight(logicalOffset, childLogicalHeight);
    if (!pageLogicalHeight || childLogicalHeight > pageLogicalHeight)
        return logicalOffset;
    LayoutUnit remainingLogicalHeight = pageRemainingLogicalHeightForOffset(logicalOffset, ExcludePageBoundary);
    if (remainingLogicalHeight < childLogicalHeight)
        return logicalOffset + remainingLogicalHeight;
    return logicalOffset;
}
