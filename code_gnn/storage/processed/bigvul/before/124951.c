void RenderBox::setOverrideLogicalContentHeight(LayoutUnit height)
{
    ASSERT(height >= 0);
    ensureRareData().m_overrideLogicalContentHeight = height;
}
