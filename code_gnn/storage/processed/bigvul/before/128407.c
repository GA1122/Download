void RenderLayerScrollableArea::invalidateScrollbarRect(Scrollbar* scrollbar, const IntRect& rect)
{
    DisableCompositingQueryAsserts disabler;

    if (scrollbar == m_vBar.get()) {
        if (GraphicsLayer* layer = layerForVerticalScrollbar()) {
            layer->setNeedsDisplayInRect(rect);
            return;
        }
    } else {
        if (GraphicsLayer* layer = layerForHorizontalScrollbar()) {
            layer->setNeedsDisplayInRect(rect);
            return;
        }
    }

    IntRect scrollRect = rect;
    if (!box().parent())
        return;

    if (scrollbar == m_vBar.get())
        scrollRect.move(verticalScrollbarStart(0, box().width()), box().borderTop());
    else
        scrollRect.move(horizontalScrollbarStart(0), box().height() - box().borderBottom() - scrollbar->height());

    if (scrollRect.isEmpty())
        return;

    LayoutRect paintInvalidationRect = scrollRect;
    box().flipForWritingMode(paintInvalidationRect);

    IntRect intRect = pixelSnappedIntRect(paintInvalidationRect);

    if (box().frameView()->isInPerformLayout())
        addScrollbarDamage(scrollbar, intRect);
    else
        box().invalidatePaintRectangle(intRect);
}
