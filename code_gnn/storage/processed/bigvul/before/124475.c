void RenderBlock::addChildToAnonymousColumnBlocks(RenderObject* newChild, RenderObject* beforeChild)
{
    ASSERT(!continuation());  

    RenderBlock* beforeChildParent = 0;
    if (beforeChild) {
        RenderObject* curr = beforeChild;
        while (curr && curr->parent() != this)
            curr = curr->parent();
        beforeChildParent = toRenderBlock(curr);
        ASSERT(beforeChildParent);
        ASSERT(beforeChildParent->isAnonymousColumnsBlock() || beforeChildParent->isAnonymousColumnSpanBlock());
    } else
        beforeChildParent = toRenderBlock(lastChild());

    if (newChild->isFloatingOrOutOfFlowPositioned()) {
        beforeChildParent->addChildIgnoringAnonymousColumnBlocks(newChild, beforeChild);
        return;
    }

    bool newChildHasColumnSpan = newChild->style()->columnSpan() && !newChild->isInline();
    bool beforeChildParentHoldsColumnSpans = beforeChildParent->isAnonymousColumnSpanBlock();

    if (newChildHasColumnSpan == beforeChildParentHoldsColumnSpans) {
        beforeChildParent->addChildIgnoringAnonymousColumnBlocks(newChild, beforeChild);
        return;
    }

    if (!beforeChild) {
        RenderBlock* newBox = newChildHasColumnSpan ? createAnonymousColumnSpanBlock() : createAnonymousColumnsBlock();
        children()->appendChildNode(this, newBox);
        newBox->addChildIgnoringAnonymousColumnBlocks(newChild, 0);
        return;
    }

    RenderObject* immediateChild = beforeChild;
    bool isPreviousBlockViable = true;
    while (immediateChild->parent() != this) {
        if (isPreviousBlockViable)
            isPreviousBlockViable = !immediateChild->previousSibling();
        immediateChild = immediateChild->parent();
    }
    if (isPreviousBlockViable && immediateChild->previousSibling()) {
        toRenderBlock(immediateChild->previousSibling())->addChildIgnoringAnonymousColumnBlocks(newChild, 0);  
        return;
    }

    RenderObject* newBeforeChild = splitAnonymousBoxesAroundChild(beforeChild);


    RenderBlock* newBox = newChildHasColumnSpan ? createAnonymousColumnSpanBlock() : createAnonymousColumnsBlock();
    children()->insertChildNode(this, newBox, newBeforeChild);
    newBox->addChildIgnoringAnonymousColumnBlocks(newChild, 0);
    return;
}
