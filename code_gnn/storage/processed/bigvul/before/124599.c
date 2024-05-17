void RenderBlock::offsetForContents(LayoutPoint& offset) const
{
    offset = flipForWritingMode(offset);

    if (hasOverflowClip())
        offset += scrolledContentOffset();

    if (hasColumns())
        adjustPointToColumnContents(offset);

    offset = flipForWritingMode(offset);
}
