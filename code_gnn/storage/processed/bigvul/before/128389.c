IntPoint RenderLayerScrollableArea::convertFromContainingViewToScrollbar(const Scrollbar* scrollbar, const IntPoint& parentPoint) const
{
    RenderView* view = box().view();
    if (!view)
        return parentPoint;

    IntPoint point = view->frameView()->convertToRenderer(box(), parentPoint);

    point.move(-scrollbarOffset(scrollbar));
    return point;
}
