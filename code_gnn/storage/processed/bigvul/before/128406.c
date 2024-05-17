void RenderLayerScrollableArea::invalidateScrollCornerRect(const IntRect& rect)
{
    if (GraphicsLayer* layer = layerForScrollCorner()) {
        layer->setNeedsDisplayInRect(rect);
        return;
    }

    if (m_scrollCorner)
        m_scrollCorner->invalidatePaintRectangle(rect);
    if (m_resizer)
        m_resizer->invalidatePaintRectangle(rect);
}
