void LayoutBlockFlow::setMaxMarginAfterValues(LayoutUnit pos, LayoutUnit neg)
{
    if (!m_rareData) {
        if (pos == LayoutBlockFlowRareData::positiveMarginAfterDefault(this) && neg == LayoutBlockFlowRareData::negativeMarginAfterDefault(this))
            return;
        m_rareData = adoptPtr(new LayoutBlockFlowRareData(this));
    }
    m_rareData->m_margins.setPositiveMarginAfter(pos);
    m_rareData->m_margins.setNegativeMarginAfter(neg);
}
