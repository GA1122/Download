IntPoint RenderLayerScrollableArea::maximumScrollPosition() const
{
    if (!box().hasOverflowClip())
        return -scrollOrigin();
    return -scrollOrigin() + IntPoint(pixelSnappedScrollWidth(), pixelSnappedScrollHeight()) - enclosingIntRect(box().clientBoxRect()).size();
}
