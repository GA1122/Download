void RenderBlock::makeChildrenAnonymousColumnBlocks(RenderObject* beforeChild, RenderBlockFlow* newBlockBox, RenderObject* newChild)
{
    RenderBlockFlow* pre = 0;
    RenderBlockFlow* post = 0;
    RenderBlock* block = this;  

    block->deleteLineBoxTree();

    if (beforeChild && beforeChild->parent() != this)
        beforeChild = splitAnonymousBoxesAroundChild(beforeChild);

    if (beforeChild != firstChild()) {
        pre = block->createAnonymousColumnsBlock();
        pre->setChildrenInline(block->childrenInline());
    }

    if (beforeChild) {
        post = block->createAnonymousColumnsBlock();
        post->setChildrenInline(block->childrenInline());
    }

    RenderObject* boxFirst = block->firstChild();
    if (pre)
        block->children()->insertChildNode(block, pre, boxFirst);
    block->children()->insertChildNode(block, newBlockBox, boxFirst);
    if (post)
        block->children()->insertChildNode(block, post, boxFirst);
    block->setChildrenInline(false);

    block->moveChildrenTo(pre, boxFirst, beforeChild, true);
    block->moveChildrenTo(post, beforeChild, 0, true);

    newBlockBox->setChildrenInline(false);

    newBlockBox->addChild(newChild);

    if (pre)
        pre->setNeedsLayoutAndPrefWidthsRecalc();
    block->setNeedsLayoutAndPrefWidthsRecalc();
    if (post)
        post->setNeedsLayoutAndPrefWidthsRecalc();
}
