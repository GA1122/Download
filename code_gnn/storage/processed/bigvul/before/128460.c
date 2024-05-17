bool RenderLayerScrollableArea::userInputScrollable(ScrollbarOrientation orientation) const
{
    if (box().isIntristicallyScrollable(orientation))
        return true;

    EOverflow overflowStyle = (orientation == HorizontalScrollbar) ?
        box().style()->overflowX() : box().style()->overflowY();
    return (overflowStyle == OSCROLL || overflowStyle == OAUTO || overflowStyle == OOVERLAY);
}
