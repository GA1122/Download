PassRefPtr<Scrollbar> RenderLayerScrollableArea::createScrollbar(ScrollbarOrientation orientation)
{
    RefPtr<Scrollbar> widget;
    RenderObject* actualRenderer = rendererForScrollbar(box());
    bool hasCustomScrollbarStyle = actualRenderer->isBox() && actualRenderer->style()->hasPseudoStyle(SCROLLBAR);
    if (hasCustomScrollbarStyle) {
        widget = RenderScrollbar::createCustomScrollbar(this, orientation, actualRenderer->node());
    } else {
        ScrollbarControlSize scrollbarSize = RegularScrollbar;
        if (actualRenderer->style()->hasAppearance())
            scrollbarSize = RenderTheme::theme().scrollbarControlSizeForPart(actualRenderer->style()->appearance());
        widget = Scrollbar::create(this, orientation, scrollbarSize);
        if (orientation == HorizontalScrollbar)
            didAddScrollbar(widget.get(), HorizontalScrollbar);
        else
            didAddScrollbar(widget.get(), VerticalScrollbar);
    }
    box().document().view()->addChild(widget.get());
    return widget.release();
}
