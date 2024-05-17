LayoutUnit RenderBlock::desiredColumnWidth() const
{
    if (!hasColumns())
        return contentLogicalWidth();
    return gColumnInfoMap->get(this)->desiredColumnWidth();
}
