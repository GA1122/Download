void FrameView::invalidateScrollbarRect(Scrollbar* scrollbar, const IntRect& rect)
{
    IntRect dirtyRect = rect;
    dirtyRect.moveBy(scrollbar->location());

    if (isInPerformLayout())
        addScrollbarDamage(scrollbar, rect);
    else
        invalidateRect(dirtyRect);
}
