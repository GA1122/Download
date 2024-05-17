LayoutUnit RenderBox::overrideLogicalContentWidth() const
{
    ASSERT(hasOverrideWidth());
    return m_rareData->m_overrideLogicalContentWidth;
}
