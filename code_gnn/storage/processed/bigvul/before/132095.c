LayoutBlockFlow::MarginValues LayoutBlockFlow::marginValuesForChild(LayoutBox& child) const
{
    LayoutUnit childBeforePositive = 0;
    LayoutUnit childBeforeNegative = 0;
    LayoutUnit childAfterPositive = 0;
    LayoutUnit childAfterNegative = 0;

    LayoutUnit beforeMargin = 0;
    LayoutUnit afterMargin = 0;

    LayoutBlockFlow* childLayoutBlockFlow = child.isLayoutBlockFlow() ? toLayoutBlockFlow(&child) : 0;

    if (!child.isWritingModeRoot()) {
        if (childLayoutBlockFlow) {
            childBeforePositive = childLayoutBlockFlow->maxPositiveMarginBefore();
            childBeforeNegative = childLayoutBlockFlow->maxNegativeMarginBefore();
            childAfterPositive = childLayoutBlockFlow->maxPositiveMarginAfter();
            childAfterNegative = childLayoutBlockFlow->maxNegativeMarginAfter();
        } else {
            beforeMargin = child.marginBefore();
            afterMargin = child.marginAfter();
        }
    } else if (child.isHorizontalWritingMode() == isHorizontalWritingMode()) {
        if (childLayoutBlockFlow) {
            childBeforePositive = childLayoutBlockFlow->maxPositiveMarginAfter();
            childBeforeNegative = childLayoutBlockFlow->maxNegativeMarginAfter();
            childAfterPositive = childLayoutBlockFlow->maxPositiveMarginBefore();
            childAfterNegative = childLayoutBlockFlow->maxNegativeMarginBefore();
        } else {
            beforeMargin = child.marginAfter();
            afterMargin = child.marginBefore();
        }
    } else {
        beforeMargin = marginBeforeForChild(child);
        afterMargin = marginAfterForChild(child);
    }

    if (beforeMargin) {
        if (beforeMargin > 0)
            childBeforePositive = beforeMargin;
        else
            childBeforeNegative = -beforeMargin;
    }
    if (afterMargin) {
        if (afterMargin > 0)
            childAfterPositive = afterMargin;
        else
            childAfterNegative = -afterMargin;
    }

    return LayoutBlockFlow::MarginValues(childBeforePositive, childBeforeNegative, childAfterPositive, childAfterNegative);
}
