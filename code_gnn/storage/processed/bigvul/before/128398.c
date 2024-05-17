bool RenderLayerScrollableArea::hasHorizontalOverflow() const
{
    ASSERT(!m_scrollDimensionsDirty);

    return pixelSnappedScrollWidth() > box().pixelSnappedClientWidth();
}
