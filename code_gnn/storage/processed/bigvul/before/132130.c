 void LayoutBlockFlow::setPaginationStrut(LayoutUnit strut)
 {
     if (!m_rareData) {
        if (!strut)
            return;
        m_rareData = adoptPtr(new LayoutBlockFlowRareData(this));
    }
    m_rareData->m_paginationStrut = strut;
}
