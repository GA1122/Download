void RenderBlock::computeBlockPreferredLogicalWidths(LayoutUnit& minLogicalWidth, LayoutUnit& maxLogicalWidth) const
{
    RenderStyle* styleToUse = style();
    bool nowrap = styleToUse->whiteSpace() == NOWRAP;

    RenderObject* child = firstChild();
    RenderBlock* containingBlock = this->containingBlock();
    LayoutUnit floatLeftWidth = 0, floatRightWidth = 0;
    while (child) {
        if (child->isOutOfFlowPositioned()) {
            child = child->nextSibling();
            continue;
        }

        RenderStyle* childStyle = child->style();
        if (child->isFloating() || (child->isBox() && toRenderBox(child)->avoidsFloats())) {
            LayoutUnit floatTotalWidth = floatLeftWidth + floatRightWidth;
            if (childStyle->clear() & CLEFT) {
                maxLogicalWidth = max(floatTotalWidth, maxLogicalWidth);
                floatLeftWidth = 0;
            }
            if (childStyle->clear() & CRIGHT) {
                maxLogicalWidth = max(floatTotalWidth, maxLogicalWidth);
                floatRightWidth = 0;
            }
        }

        Length startMarginLength = childStyle->marginStartUsing(styleToUse);
        Length endMarginLength = childStyle->marginEndUsing(styleToUse);
        LayoutUnit margin = 0;
        LayoutUnit marginStart = 0;
        LayoutUnit marginEnd = 0;
        if (startMarginLength.isFixed())
            marginStart += startMarginLength.value();
        if (endMarginLength.isFixed())
            marginEnd += endMarginLength.value();
        margin = marginStart + marginEnd;

        LayoutUnit childMinPreferredLogicalWidth, childMaxPreferredLogicalWidth;
        if (child->isBox() && child->isHorizontalWritingMode() != isHorizontalWritingMode()) {
            RenderBox* childBox = toRenderBox(child);
            LogicalExtentComputedValues computedValues;
            childBox->computeLogicalHeight(childBox->borderAndPaddingLogicalHeight(), 0, computedValues);
            childMinPreferredLogicalWidth = childMaxPreferredLogicalWidth = computedValues.m_extent;
        } else {
            childMinPreferredLogicalWidth = child->minPreferredLogicalWidth();
            childMaxPreferredLogicalWidth = child->maxPreferredLogicalWidth();
        }

        LayoutUnit w = childMinPreferredLogicalWidth + margin;
        minLogicalWidth = max(w, minLogicalWidth);

        if (nowrap && !child->isTable())
            maxLogicalWidth = max(w, maxLogicalWidth);

        w = childMaxPreferredLogicalWidth + margin;

        if (!child->isFloating()) {
            if (child->isBox() && toRenderBox(child)->avoidsFloats()) {
                bool ltr = containingBlock ? containingBlock->style()->isLeftToRightDirection() : styleToUse->isLeftToRightDirection();
                LayoutUnit marginLogicalLeft = ltr ? marginStart : marginEnd;
                LayoutUnit marginLogicalRight = ltr ? marginEnd : marginStart;
                LayoutUnit maxLeft = marginLogicalLeft > 0 ? max(floatLeftWidth, marginLogicalLeft) : floatLeftWidth + marginLogicalLeft;
                LayoutUnit maxRight = marginLogicalRight > 0 ? max(floatRightWidth, marginLogicalRight) : floatRightWidth + marginLogicalRight;
                w = childMaxPreferredLogicalWidth + maxLeft + maxRight;
                w = max(w, floatLeftWidth + floatRightWidth);
            }
            else
                maxLogicalWidth = max(floatLeftWidth + floatRightWidth, maxLogicalWidth);
            floatLeftWidth = floatRightWidth = 0;
        }

        if (child->isFloating()) {
            if (childStyle->floating() == LeftFloat)
                floatLeftWidth += w;
            else
                floatRightWidth += w;
        } else
            maxLogicalWidth = max(w, maxLogicalWidth);

        child = child->nextSibling();
    }

    minLogicalWidth = max<LayoutUnit>(0, minLogicalWidth);
    maxLogicalWidth = max<LayoutUnit>(0, maxLogicalWidth);

    maxLogicalWidth = max(floatLeftWidth + floatRightWidth, maxLogicalWidth);
}
