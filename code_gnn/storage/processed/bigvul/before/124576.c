int RenderBlock::lastLineBoxBaseline(LineDirectionMode lineDirection) const
{
    if (isWritingModeRoot() && !isRubyRun())
        return -1;

    if (childrenInline()) {
        if (!firstLineBox() && hasLineIfEmpty()) {
            const FontMetrics& fontMetrics = firstLineStyle()->fontMetrics();
            return fontMetrics.ascent()
                 + (lineHeight(true, lineDirection, PositionOfInteriorLineBoxes) - fontMetrics.height()) / 2
                 + (lineDirection == HorizontalLine ? borderTop() + paddingTop() : borderRight() + paddingRight());
        }
        if (lastLineBox())
            return lastLineBox()->logicalTop() + style(lastLineBox() == firstLineBox())->fontMetrics().ascent(lastRootBox()->baselineType());
        return -1;
    } else {
        bool haveNormalFlowChild = false;
        for (RenderBox* curr = lastChildBox(); curr; curr = curr->previousSiblingBox()) {
            if (!curr->isFloatingOrOutOfFlowPositioned()) {
                haveNormalFlowChild = true;
                int result = curr->inlineBlockBaseline(lineDirection);
                if (result != -1)
                    return curr->logicalTop() + result;  
            }
        }
        if (!haveNormalFlowChild && hasLineIfEmpty()) {
            const FontMetrics& fontMetrics = firstLineStyle()->fontMetrics();
            return fontMetrics.ascent()
                 + (lineHeight(true, lineDirection, PositionOfInteriorLineBoxes) - fontMetrics.height()) / 2
                 + (lineDirection == HorizontalLine ? borderTop() + paddingTop() : borderRight() + paddingRight());
        }
    }

    return -1;
}
