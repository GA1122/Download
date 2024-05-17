void LayoutBlockFlow::setMaxMarginBeforeValues(LayoutUnit pos, LayoutUnit neg)
{
    if (!m_rareData) {
        if (pos == LayoutBlockFlowRareData::positiveMarginBeforeDefault(this) && neg == LayoutBlockFlowRareData::negativeMarginBeforeDefault(this))
            return;
        m_rareData = adoptPtr(new LayoutBlockFlowRareData(this));
    }
    m_rareData->m_margins.setPositiveMarginBefore(pos);
    m_rareData->m_margins.setNegativeMarginBefore(neg);
}
