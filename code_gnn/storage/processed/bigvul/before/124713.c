RenderBlockFlow::RenderBlockFlowRareData& RenderBlockFlow::ensureRareData()
{
    if (m_rareData)
        return *m_rareData;

    m_rareData = adoptPtr(new RenderBlockFlowRareData(this));
    return *m_rareData;
}
