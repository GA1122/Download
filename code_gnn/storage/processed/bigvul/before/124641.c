void RenderBlock::setPageLogicalOffset(LayoutUnit logicalOffset)
{
    if (!m_rareData) {
        if (!logicalOffset)
            return;
        m_rareData = adoptPtr(new RenderBlockRareData());
    }
    m_rareData->m_pageLogicalOffset = logicalOffset;
}
