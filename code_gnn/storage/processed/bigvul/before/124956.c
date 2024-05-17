LayoutUnit RenderBox::shrinkLogicalWidthToAvoidFloats(LayoutUnit childMarginStart, LayoutUnit childMarginEnd, const RenderBlockFlow* cb) const
{
    LayoutUnit logicalTopPosition = logicalTop();
    LayoutUnit result = cb->availableLogicalWidthForLine(logicalTopPosition, false) - childMarginStart - childMarginEnd;

    if (childMarginStart > 0) {
        LayoutUnit startContentSide = cb->startOffsetForContent();
        LayoutUnit startContentSideWithMargin = startContentSide + childMarginStart;
        LayoutUnit startOffset = cb->startOffsetForLine(logicalTopPosition, false);
        if (startOffset > startContentSideWithMargin)
            result += childMarginStart;
        else
            result += startOffset - startContentSide;
    }

    if (childMarginEnd > 0) {
        LayoutUnit endContentSide = cb->endOffsetForContent();
        LayoutUnit endContentSideWithMargin = endContentSide + childMarginEnd;
        LayoutUnit endOffset = cb->endOffsetForLine(logicalTopPosition, false);
        if (endOffset > endContentSideWithMargin)
            result += childMarginEnd;
        else
            result += endOffset - endContentSide;
    }

    return result;
}
