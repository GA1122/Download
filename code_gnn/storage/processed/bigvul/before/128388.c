IntRect RenderLayerScrollableArea::convertFromContainingViewToScrollbar(const Scrollbar* scrollbar, const IntRect& parentRect) const
{
    RenderView* view = box().view();
    if (!view)
        return parentRect;

    IntRect rect = view->frameView()->convertToRenderer(box(), parentRect);
    rect.move(-scrollbarOffset(scrollbar));
    return rect;
}
