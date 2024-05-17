void RenderBox::setOverrideLogicalContentWidth(LayoutUnit width)
{
    ASSERT(width >= 0);
    ensureRareData().m_overrideLogicalContentWidth = width;
}
