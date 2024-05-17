LayoutUnit RenderLayerScrollableArea::horizontalScrollbarStart(int minX) const
{
    int x = minX + box().borderLeft();
    if (box().style()->shouldPlaceBlockDirectionScrollbarOnLogicalLeft())
        x += m_vBar ? m_vBar->width() : resizerCornerRect(box().pixelSnappedBorderBoxRect(), ResizerForPointer).width();
    return x;
}
