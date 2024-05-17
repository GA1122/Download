LayoutUnit RenderFlexibleBox::autoMarginOffsetInMainAxis(const OrderedFlexItemList& children, LayoutUnit& availableFreeSpace)
{
    if (availableFreeSpace <= 0)
        return 0;

    int numberOfAutoMargins = 0;
    bool isHorizontal = isHorizontalFlow();
    for (size_t i = 0; i < children.size(); ++i) {
        RenderBox* child = children[i];
        if (child->isOutOfFlowPositioned())
            continue;
        if (isHorizontal) {
            if (child->style()->marginLeft().isAuto())
                ++numberOfAutoMargins;
            if (child->style()->marginRight().isAuto())
                ++numberOfAutoMargins;
        } else {
            if (child->style()->marginTop().isAuto())
                ++numberOfAutoMargins;
            if (child->style()->marginBottom().isAuto())
                ++numberOfAutoMargins;
        }
    }
    if (!numberOfAutoMargins)
        return 0;

    LayoutUnit sizeOfAutoMargin = availableFreeSpace / numberOfAutoMargins;
    availableFreeSpace = 0;
    return sizeOfAutoMargin;
}
