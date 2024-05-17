RenderBlockFlow::MarginValues RenderBlockFlow::marginValuesForChild(RenderBox* child) const
{
    LayoutUnit childBeforePositive = 0;
    LayoutUnit childBeforeNegative = 0;
    LayoutUnit childAfterPositive = 0;
    LayoutUnit childAfterNegative = 0;

    LayoutUnit beforeMargin = 0;
    LayoutUnit afterMargin = 0;

    RenderBlockFlow* childRenderBlockFlow = child->isRenderBlockFlow() ? toRenderBlockFlow(child) : 0;

    if (!child->isWritingModeRoot()) {
        if (childRenderBlockFlow) {
            childBeforePositive = childRenderBlockFlow->maxPositiveMarginBefore();
            childBeforeNegative = childRenderBlockFlow->maxNegativeMarginBefore();
            childAfterPositive = childRenderBlockFlow->maxPositiveMarginAfter();
            childAfterNegative = childRenderBlockFlow->maxNegativeMarginAfter();
        } else {
            beforeMargin = child->marginBefore();
            afterMargin = child->marginAfter();
        }
    } else if (child->isHorizontalWritingMode() == isHorizontalWritingMode()) {
        if (childRenderBlockFlow) {
            childBeforePositive = childRenderBlockFlow->maxPositiveMarginAfter();
            childBeforeNegative = childRenderBlockFlow->maxNegativeMarginAfter();
            childAfterPositive = childRenderBlockFlow->maxPositiveMarginBefore();
            childAfterNegative = childRenderBlockFlow->maxNegativeMarginBefore();
        } else {
            beforeMargin = child->marginAfter();
            afterMargin = child->marginBefore();
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

    return RenderBlockFlow::MarginValues(childBeforePositive, childBeforeNegative, childAfterPositive, childAfterNegative);
}
