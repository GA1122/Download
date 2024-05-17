inline static bool percentageLogicalHeightIsResolvable(const RenderBox* box)
{
    return RenderBox::percentageLogicalHeightIsResolvableFromBlock(box->containingBlock(), box->isOutOfFlowPositioned());
}
