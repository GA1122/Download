RenderBlockFlow::RenderBlockFlow(ContainerNode* node)
    : RenderBlock(node)
{
    COMPILE_ASSERT(sizeof(MarginInfo) == sizeof(SameSizeAsMarginInfo), MarginInfo_should_stay_small);
}
