LayoutUnit RenderBox::overrideLogicalContentHeight() const
{
    ASSERT(hasOverrideHeight());
    return m_rareData->m_overrideLogicalContentHeight;
}
