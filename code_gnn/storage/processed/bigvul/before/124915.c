void RenderBox::paintBoxDecorations(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    if (!paintInfo.shouldPaintWithinRoot(this))
        return;

    LayoutRect paintRect = borderBoxRect();
    paintRect.moveBy(paintOffset);
    paintBoxDecorationsWithRect(paintInfo, paintOffset, paintRect);
}
