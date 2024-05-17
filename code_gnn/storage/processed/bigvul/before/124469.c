RenderBlock::RenderBlock(ContainerNode* node)
    : RenderBox(node)
    , m_lineHeight(-1)
    , m_hasMarginBeforeQuirk(false)
    , m_hasMarginAfterQuirk(false)
    , m_beingDestroyed(false)
    , m_hasMarkupTruncation(false)
    , m_hasBorderOrPaddingLogicalWidthChanged(false)
    , m_hasOnlySelfCollapsingChildren(false)
{
    setChildrenInline(true);
}
