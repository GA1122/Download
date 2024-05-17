void LayoutBlockFlow::positionSpannerDescendant(LayoutMultiColumnSpannerPlaceholder& child)
{
    LayoutBox& spanner = *child.layoutObjectInFlowThread();
    setLogicalTopForChild(spanner, child.logicalTop());
    determineLogicalLeftPositionForChild(spanner);
}
