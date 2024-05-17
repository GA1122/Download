bool RenderFlexibleBox::resolveFlexibleLengths(FlexSign flexSign, const OrderedFlexItemList& children, LayoutUnit& availableFreeSpace, double& totalFlexGrow, double& totalWeightedFlexShrink, InflexibleFlexItemSize& inflexibleItems, Vector<LayoutUnit, 16>& childSizes, bool hasInfiniteLineLength)
{
    childSizes.resize(0);
    LayoutUnit totalViolation = 0;
    LayoutUnit usedFreeSpace = 0;
    Vector<Violation> minViolations;
    Vector<Violation> maxViolations;
    for (size_t i = 0; i < children.size(); ++i) {
        RenderBox* child = children[i];
        if (child->isOutOfFlowPositioned()) {
            childSizes.append(0);
            continue;
        }

        if (inflexibleItems.contains(child))
            childSizes.append(inflexibleItems.get(child));
        else {
            LayoutUnit preferredChildSize = preferredMainAxisContentExtentForChild(child, hasInfiniteLineLength);
            LayoutUnit childSize = preferredChildSize;
            double extraSpace = 0;
            if (availableFreeSpace > 0 && totalFlexGrow > 0 && flexSign == PositiveFlexibility && std::isfinite(totalFlexGrow))
                extraSpace = availableFreeSpace * child->style()->flexGrow() / totalFlexGrow;
            else if (availableFreeSpace < 0 && totalWeightedFlexShrink > 0 && flexSign == NegativeFlexibility && std::isfinite(totalWeightedFlexShrink))
                extraSpace = availableFreeSpace * child->style()->flexShrink() * preferredChildSize / totalWeightedFlexShrink;
            if (std::isfinite(extraSpace))
                childSize += LayoutUnit::fromFloatRound(extraSpace);

            LayoutUnit adjustedChildSize = adjustChildSizeForMinAndMax(child, childSize);
            childSizes.append(adjustedChildSize);
            usedFreeSpace += adjustedChildSize - preferredChildSize;

            LayoutUnit violation = adjustedChildSize - childSize;
            if (violation > 0)
                minViolations.append(Violation(child, adjustedChildSize));
            else if (violation < 0)
                maxViolations.append(Violation(child, adjustedChildSize));
            totalViolation += violation;
        }
    }

    if (totalViolation)
        freezeViolations(totalViolation < 0 ? maxViolations : minViolations, availableFreeSpace, totalFlexGrow, totalWeightedFlexShrink, inflexibleItems, hasInfiniteLineLength);
    else
        availableFreeSpace -= usedFreeSpace;

    return !totalViolation;
}
