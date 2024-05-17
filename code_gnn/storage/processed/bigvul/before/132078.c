bool LayoutBlockFlow::isSelfCollapsingBlock() const
{
    m_hasOnlySelfCollapsingChildren = LayoutBlock::isSelfCollapsingBlock();
    return m_hasOnlySelfCollapsingChildren;
}
