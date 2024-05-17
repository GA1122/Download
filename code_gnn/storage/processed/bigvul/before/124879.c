bool RenderBox::hasOverrideHeight() const
{
    return m_rareData && m_rareData->m_overrideLogicalContentHeight != -1;
}
