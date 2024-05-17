void RenderBlock::adjustForColumnRect(LayoutSize& offset, const LayoutPoint& locationInContainer) const
{
    for (ColumnRectIterator it(*this); it.hasMore(); it.advance()) {
        LayoutRect colRect = it.columnRect();
        if (colRect.contains(locationInContainer)) {
            it.adjust(offset);
            return;
        }
    }
}
