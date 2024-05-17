IntSize RenderLayerScrollableArea::scrollbarOffset(const Scrollbar* scrollbar) const
{
    if (scrollbar == m_vBar.get())
        return IntSize(verticalScrollbarStart(0, box().width()), box().borderTop());

    if (scrollbar == m_hBar.get())
        return IntSize(horizontalScrollbarStart(0), box().height() - box().borderBottom() - scrollbar->height());

    ASSERT_NOT_REACHED();
    return IntSize();
}
