void RenderBox::addVisualEffectOverflow()
{
    if (!style()->boxShadow() && !style()->hasBorderImageOutsets() && !style()->hasOutline())
        return;

    bool isFlipped = style()->isFlippedBlocksWritingMode();
    bool isHorizontal = isHorizontalWritingMode();

    LayoutRect borderBox = borderBoxRect();
    LayoutUnit overflowMinX = borderBox.x();
    LayoutUnit overflowMaxX = borderBox.maxX();
    LayoutUnit overflowMinY = borderBox.y();
    LayoutUnit overflowMaxY = borderBox.maxY();

    if (style()->boxShadow()) {
        LayoutUnit shadowLeft;
        LayoutUnit shadowRight;
        LayoutUnit shadowTop;
        LayoutUnit shadowBottom;
        style()->getBoxShadowExtent(shadowTop, shadowRight, shadowBottom, shadowLeft);

        overflowMinX = borderBox.x() + ((!isFlipped || isHorizontal) ? shadowLeft : -shadowRight);
        overflowMaxX = borderBox.maxX() + ((!isFlipped || isHorizontal) ? shadowRight : -shadowLeft);
        overflowMinY = borderBox.y() + ((!isFlipped || !isHorizontal) ? shadowTop : -shadowBottom);
        overflowMaxY = borderBox.maxY() + ((!isFlipped || !isHorizontal) ? shadowBottom : -shadowTop);
    }

    if (style()->hasBorderImageOutsets()) {
        LayoutBoxExtent borderOutsets = style()->borderImageOutsets();

        overflowMinX = min(overflowMinX, borderBox.x() - ((!isFlipped || isHorizontal) ? borderOutsets.left() : borderOutsets.right()));
        overflowMaxX = max(overflowMaxX, borderBox.maxX() + ((!isFlipped || isHorizontal) ? borderOutsets.right() : borderOutsets.left()));
        overflowMinY = min(overflowMinY, borderBox.y() - ((!isFlipped || !isHorizontal) ? borderOutsets.top() : borderOutsets.bottom()));
        overflowMaxY = max(overflowMaxY, borderBox.maxY() + ((!isFlipped || !isHorizontal) ? borderOutsets.bottom() : borderOutsets.top()));
    }

    if (style()->hasOutline()) {
        LayoutUnit outlineSize = style()->outlineSize();

        overflowMinX = min(overflowMinX, borderBox.x() - outlineSize);
        overflowMaxX = max(overflowMaxX, borderBox.maxX() + outlineSize);
        overflowMinY = min(overflowMinY, borderBox.y() - outlineSize);
        overflowMaxY = max(overflowMaxY, borderBox.maxY() + outlineSize);
    }

    LayoutRect visualEffectOverflow(overflowMinX, overflowMinY, overflowMaxX - overflowMinX, overflowMaxY - overflowMinY);
    addVisualOverflow(visualEffectOverflow);
}
