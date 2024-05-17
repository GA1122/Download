LayoutUnit LayoutBlockFlow::adjustForUnsplittableChild(LayoutBox& child, LayoutUnit logicalOffset)
{
    bool checkColumnBreaks = flowThreadContainingBlock();
    bool checkPageBreaks = !checkColumnBreaks && view()->layoutState()->pageLogicalHeight();
    bool isUnsplittable = child.isUnsplittableForPagination() || (checkColumnBreaks && child.style()->columnBreakInside() == PBAVOID)
        || (checkPageBreaks && child.style()->pageBreakInside() == PBAVOID);
    if (!isUnsplittable)
        return logicalOffset;
    LayoutUnit childLogicalHeight = logicalHeightForChild(child);
    if (child.isFloating())
        childLogicalHeight += marginBeforeForChild(child) + marginAfterForChild(child);
    LayoutUnit pageLogicalHeight = pageLogicalHeightForOffset(logicalOffset);
    updateMinimumPageHeight(logicalOffset, childLogicalHeight);
    if (!pageLogicalHeight)
        return logicalOffset;
    LayoutUnit remainingLogicalHeight = pageRemainingLogicalHeightForOffset(logicalOffset, AssociateWithLatterPage);
    if (remainingLogicalHeight < childLogicalHeight && remainingLogicalHeight < pageLogicalHeight)
        return logicalOffset + remainingLogicalHeight;
    return logicalOffset;
}
