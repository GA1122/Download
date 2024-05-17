LayoutRect RenderBox::noOverflowRect() const
{

    const int scrollBarWidth = verticalScrollbarWidth();
    const int scrollBarHeight = horizontalScrollbarHeight();
    LayoutUnit left = borderLeft() + (style()->shouldPlaceBlockDirectionScrollbarOnLogicalLeft() ? scrollBarWidth : 0);
    LayoutUnit top = borderTop();
    LayoutUnit right = borderRight();
    LayoutUnit bottom = borderBottom();
    LayoutRect rect(left, top, width() - left - right, height() - top - bottom);
    flipForWritingMode(rect);
    if (style()->shouldPlaceBlockDirectionScrollbarOnLogicalLeft())
        rect.contract(0, scrollBarHeight);
    else
        rect.contract(scrollBarWidth, scrollBarHeight);
    return rect;
}
