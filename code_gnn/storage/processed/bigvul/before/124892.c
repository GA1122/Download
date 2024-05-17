LayoutRect RenderBox::localCaretRect(InlineBox* box, int caretOffset, LayoutUnit* extraWidthToEndOfLine)
{

    LayoutRect rect(location(), LayoutSize(caretWidth, height()));
    bool ltr = box ? box->isLeftToRightDirection() : style()->isLeftToRightDirection();

    if ((!caretOffset) ^ ltr)
        rect.move(LayoutSize(width() - caretWidth, 0));

    if (box) {
        RootInlineBox& rootBox = box->root();
        LayoutUnit top = rootBox.lineTop();
        rect.setY(top);
        rect.setHeight(rootBox.lineBottom() - top);
    }

    LayoutUnit fontHeight = style()->fontMetrics().height();
    if (fontHeight > rect.height() || (!isReplaced() && !isTable()))
        rect.setHeight(fontHeight);

    if (extraWidthToEndOfLine)
        *extraWidthToEndOfLine = x() + width() - rect.maxX();

    rect.moveBy(-location());

    if (node() && !(editingIgnoresContent(node()) || isRenderedTable(node()))) {
        rect.setX(rect.x() + borderLeft() + paddingLeft());
        rect.setY(rect.y() + paddingTop() + borderTop());
    }

    if (!isHorizontalWritingMode())
        return rect.transposedRect();

    return rect;
}
