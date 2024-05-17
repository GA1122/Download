void RenderFlexibleBox::freezeViolations(const Vector<Violation>& violations, LayoutUnit& availableFreeSpace, double& totalFlexGrow, double& totalWeightedFlexShrink, InflexibleFlexItemSize& inflexibleItems, bool hasInfiniteLineLength)
{
    for (size_t i = 0; i < violations.size(); ++i) {
        RenderBox* child = violations[i].child;
        LayoutUnit childSize = violations[i].childSize;
        LayoutUnit preferredChildSize = preferredMainAxisContentExtentForChild(child, hasInfiniteLineLength);
        availableFreeSpace -= childSize - preferredChildSize;
        totalFlexGrow -= child->style()->flexGrow();
        totalWeightedFlexShrink -= child->style()->flexShrink() * preferredChildSize;
        inflexibleItems.set(child, childSize);
    }
}
