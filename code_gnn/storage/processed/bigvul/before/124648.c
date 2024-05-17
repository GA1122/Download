void RenderBlock::splitBlocks(RenderBlock* fromBlock, RenderBlock* toBlock,
                              RenderBlock* middleBlock,
                              RenderObject* beforeChild, RenderBoxModelObject* oldCont)
{
    RenderBlock* cloneBlock = clone();
    if (!isAnonymousBlock())
        cloneBlock->setContinuation(oldCont);

    if (!beforeChild && isAfterContent(lastChild()))
        beforeChild = lastChild();

    if (beforeChild && childrenInline())
        deleteLineBoxTree();

    moveChildrenTo(cloneBlock, beforeChild, 0, true);

    if (!cloneBlock->isAnonymousBlock())
        middleBlock->setContinuation(cloneBlock);

    RenderBoxModelObject* curr = toRenderBoxModelObject(parent());
    RenderBoxModelObject* currChild = this;
    RenderObject* currChildNextSibling = currChild->nextSibling();

    while (curr && curr->isDescendantOf(fromBlock) && curr != fromBlock) {
        ASSERT_WITH_SECURITY_IMPLICATION(curr->isRenderBlock());

        RenderBlock* blockCurr = toRenderBlock(curr);

        RenderBlock* cloneChild = cloneBlock;
        cloneBlock = blockCurr->clone();

        cloneBlock->addChildIgnoringContinuation(cloneChild, 0);

        if (!blockCurr->isAnonymousBlock()) {
            oldCont = blockCurr->continuation();
            blockCurr->setContinuation(cloneBlock);
            cloneBlock->setContinuation(oldCont);
        }

        blockCurr->moveChildrenTo(cloneBlock, currChildNextSibling, 0, true);

        currChild = curr;
        currChildNextSibling = currChild->nextSibling();
        curr = toRenderBoxModelObject(curr->parent());
    }

    toBlock->children()->appendChildNode(toBlock, cloneBlock);

    fromBlock->moveChildrenTo(toBlock, currChildNextSibling, 0, true);
}
