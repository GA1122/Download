LayoutRect RenderBox::overflowClipRect(const LayoutPoint& location, OverlayScrollbarSizeRelevancy relevancy)
{
    LayoutRect clipRect = borderBoxRect();
    clipRect.setLocation(location + clipRect.location() + LayoutSize(borderLeft(), borderTop()));
    clipRect.setSize(clipRect.size() - LayoutSize(borderLeft() + borderRight(), borderTop() + borderBottom()));

    if (!hasOverflowClip())
        return clipRect;

    if (style()->shouldPlaceBlockDirectionScrollbarOnLogicalLeft())
        clipRect.move(layer()->scrollableArea()->verticalScrollbarWidth(relevancy), 0);
    clipRect.contract(layer()->scrollableArea()->verticalScrollbarWidth(relevancy), layer()->scrollableArea()->horizontalScrollbarHeight(relevancy));

    return clipRect;
}
