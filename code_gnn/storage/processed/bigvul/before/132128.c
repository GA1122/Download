void LayoutBlockFlow::setMustDiscardMarginAfter(bool value)
{
    if (style()->marginAfterCollapse() == MDISCARD) {
        ASSERT(value);
        return;
    }

    if (!m_rareData && !value)
        return;

    if (!m_rareData)
        m_rareData = adoptPtr(new LayoutBlockFlowRareData(this));

    m_rareData->m_discardMarginAfter = value;
}
