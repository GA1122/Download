bool RenderBox::hasOverrideWidth() const
{
    return m_rareData && m_rareData->m_overrideLogicalContentWidth != -1;
}
