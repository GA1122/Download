RenderBlockFlow* RenderBlock::containingColumnsBlock(bool allowAnonymousColumnBlock)
{
    RenderBlock* firstChildIgnoringAnonymousWrappers = 0;
    for (RenderObject* curr = this; curr; curr = curr->parent()) {
        if (!curr->isRenderBlock() || curr->isFloatingOrOutOfFlowPositioned() || curr->isTableCell() || curr->isDocumentElement() || curr->isRenderView() || curr->hasOverflowClip()
            || curr->isInlineBlockOrInlineTable())
            return 0;

        if (!curr->isRenderBlockFlow() || curr->isListItem())
            return 0;

        RenderBlockFlow* currBlock = toRenderBlockFlow(curr);
        if (!currBlock->createsAnonymousWrapper())
            firstChildIgnoringAnonymousWrappers = currBlock;

        if (currBlock->style()->specifiesColumns() && (allowAnonymousColumnBlock || !currBlock->isAnonymousColumnsBlock()))
            return toRenderBlockFlow(firstChildIgnoringAnonymousWrappers);

        if (currBlock->isAnonymousColumnSpanBlock())
            return 0;
    }
    return 0;
}
