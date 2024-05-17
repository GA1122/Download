void RenderBlock::setPaginationStrut(LayoutUnit strut)
{
    if (!m_rareData) {
        if (!strut)
            return;
        m_rareData = adoptPtr(new RenderBlockRareData());
    }
    m_rareData->m_paginationStrut = strut;
}
