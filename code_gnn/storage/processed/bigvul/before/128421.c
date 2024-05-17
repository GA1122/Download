bool RenderLayerScrollableArea::overflowControlsIntersectRect(const IntRect& localRect) const
{
    const IntRect borderBox = box().pixelSnappedBorderBoxRect();

    if (rectForHorizontalScrollbar(borderBox).intersects(localRect))
        return true;

    if (rectForVerticalScrollbar(borderBox).intersects(localRect))
        return true;

    if (scrollCornerRect().intersects(localRect))
        return true;

    if (resizerCornerRect(borderBox, ResizerForPointer).intersects(localRect))
        return true;

    return false;
}
