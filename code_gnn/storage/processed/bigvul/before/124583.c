LayoutRect RenderBlock::localCaretRect(InlineBox* inlineBox, int caretOffset, LayoutUnit* extraWidthToEndOfLine)
{
    if (firstChild())
        return RenderBox::localCaretRect(inlineBox, caretOffset, extraWidthToEndOfLine);

    LayoutRect caretRect = localCaretRectForEmptyElement(width(), textIndentOffset());

    if (extraWidthToEndOfLine) {
        if (isRenderBlock()) {
            *extraWidthToEndOfLine = width() - caretRect.maxX();
        } else {

            LayoutUnit myRight = caretRect.maxX();
            FloatPoint absRightPoint = localToAbsolute(FloatPoint(myRight.toFloat(), 0));

            LayoutUnit containerRight = containingBlock()->x() + containingBlockLogicalWidthForContent();
            FloatPoint absContainerPoint = localToAbsolute(FloatPoint(containerRight.toFloat(), 0));

            *extraWidthToEndOfLine = absContainerPoint.x() - absRightPoint.x();
        }
    }

    return caretRect;
}
