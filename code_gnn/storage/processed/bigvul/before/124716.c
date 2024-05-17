LayoutUnit RenderBlockFlow::getClearDelta(RenderBox* child, LayoutUnit logicalTop)
{
    if (!containsFloats())
        return 0;

    bool clearSet = child->style()->clear() != CNONE;
    LayoutUnit logicalBottom = 0;
    switch (child->style()->clear()) {
    case CNONE:
        break;
    case CLEFT:
        logicalBottom = lowestFloatLogicalBottom(FloatingObject::FloatLeft);
        break;
    case CRIGHT:
        logicalBottom = lowestFloatLogicalBottom(FloatingObject::FloatRight);
        break;
    case CBOTH:
        logicalBottom = lowestFloatLogicalBottom();
        break;
    }

    LayoutUnit result = clearSet ? max<LayoutUnit>(0, logicalBottom - logicalTop) : LayoutUnit();
    if (!result && child->avoidsFloats()) {
        LayoutUnit newLogicalTop = logicalTop;
        while (true) {
            LayoutUnit availableLogicalWidthAtNewLogicalTopOffset = availableLogicalWidthForLine(newLogicalTop, false, logicalHeightForChild(child));
            if (availableLogicalWidthAtNewLogicalTopOffset == availableLogicalWidthForContent())
                return newLogicalTop - logicalTop;

            LayoutRect borderBox = child->borderBoxRect();
            LayoutUnit childLogicalWidthAtOldLogicalTopOffset = isHorizontalWritingMode() ? borderBox.width() : borderBox.height();

            LayoutUnit childOldLogicalWidth = child->logicalWidth();
            LayoutUnit childOldMarginLeft = child->marginLeft();
            LayoutUnit childOldMarginRight = child->marginRight();
            LayoutUnit childOldLogicalTop = child->logicalTop();

            child->setLogicalTop(newLogicalTop);
            child->updateLogicalWidth();
            borderBox = child->borderBoxRect();
            LayoutUnit childLogicalWidthAtNewLogicalTopOffset = isHorizontalWritingMode() ? borderBox.width() : borderBox.height();

            child->setLogicalTop(childOldLogicalTop);
            child->setLogicalWidth(childOldLogicalWidth);
            child->setMarginLeft(childOldMarginLeft);
            child->setMarginRight(childOldMarginRight);

            if (childLogicalWidthAtNewLogicalTopOffset <= availableLogicalWidthAtNewLogicalTopOffset) {
                if (childLogicalWidthAtOldLogicalTopOffset != childLogicalWidthAtNewLogicalTopOffset)
                    child->setChildNeedsLayout(MarkOnlyThis);
                return newLogicalTop - logicalTop;
            }

            newLogicalTop = nextFloatLogicalBottomBelow(newLogicalTop);
            ASSERT(newLogicalTop >= logicalTop);
            if (newLogicalTop < logicalTop)
                break;
        }
        ASSERT_NOT_REACHED();
    }
    return result;
}
