LayoutRect RenderBox::overflowRectForPaintRejection() const
{
    LayoutRect overflowRect = visualOverflowRect();
    if (!m_overflow || !usesCompositedScrolling())
        return overflowRect;

    overflowRect.unite(layoutOverflowRect());
    overflowRect.move(-scrolledContentOffset());
    return overflowRect;
}
