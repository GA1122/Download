bool RenderBlockFlow::isSelfCollapsingBlock() const
{
    m_hasOnlySelfCollapsingChildren = RenderBlock::isSelfCollapsingBlock();
    return m_hasOnlySelfCollapsingChildren;
}
