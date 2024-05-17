void RenderBox::computeAndSetBlockDirectionMargins(const RenderBlock* containingBlock)
{
    LayoutUnit marginBefore;
    LayoutUnit marginAfter;
    computeBlockDirectionMargins(containingBlock, marginBefore, marginAfter);
    containingBlock->setMarginBeforeForChild(this, marginBefore);
    containingBlock->setMarginAfterForChild(this, marginAfter);
}
