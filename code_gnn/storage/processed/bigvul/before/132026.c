LayoutBlockFlow::LayoutBlockFlow(ContainerNode* node)
    : LayoutBlock(node)
{
    static_assert(sizeof(MarginInfo) == sizeof(SameSizeAsMarginInfo), "MarginInfo should stay small");
    setChildrenInline(true);
}
