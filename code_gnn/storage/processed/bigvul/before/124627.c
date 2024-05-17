void RenderBlock::removeChild(RenderObject* oldChild)
{
    if (documentBeingDestroyed()) {
        RenderBox::removeChild(oldChild);
        return;
    }

    TemporaryChange<bool> columnFlowSplitEnabled(gColumnFlowSplitEnabled, false);

    RenderObject* prev = oldChild->previousSibling();
    RenderObject* next = oldChild->nextSibling();
    bool canMergeAnonymousBlocks = canMergeContiguousAnonymousBlocks(oldChild, prev, next);
    if (canMergeAnonymousBlocks && prev && next) {
        prev->setNeedsLayoutAndPrefWidthsRecalc();
        RenderBlockFlow* nextBlock = toRenderBlockFlow(next);
        RenderBlockFlow* prevBlock = toRenderBlockFlow(prev);

        if (prev->childrenInline() != next->childrenInline()) {
            RenderBlock* inlineChildrenBlock = prev->childrenInline() ? prevBlock : nextBlock;
            RenderBlock* blockChildrenBlock = prev->childrenInline() ? nextBlock : prevBlock;

            ASSERT(!inlineChildrenBlock->continuation());
            RefPtr<RenderStyle> newStyle = RenderStyle::createAnonymousStyleWithDisplay(style(), BLOCK);
            bool inlineChildrenBlockHasLayer = inlineChildrenBlock->hasLayer();
            inlineChildrenBlock->setStyle(newStyle);
            children()->removeChildNode(this, inlineChildrenBlock, inlineChildrenBlockHasLayer);

            blockChildrenBlock->children()->insertChildNode(blockChildrenBlock, inlineChildrenBlock, prev == inlineChildrenBlock ? blockChildrenBlock->firstChild() : 0,
                                                            inlineChildrenBlockHasLayer || blockChildrenBlock->hasLayer());
            next->setNeedsLayoutAndPrefWidthsRecalc();

            if (inlineChildrenBlock == prevBlock)
                prev = 0;
            else
                next = 0;
        } else {
            nextBlock->moveAllChildrenIncludingFloatsTo(prevBlock, nextBlock->hasLayer() || prevBlock->hasLayer());

            nextBlock->deleteLineBoxTree();
            nextBlock->destroy();
            next = 0;
        }
    }

    RenderBox::removeChild(oldChild);

    RenderObject* child = prev ? prev : next;
    if (canMergeAnonymousBlocks && child && !child->previousSibling() && !child->nextSibling() && canCollapseAnonymousBlockChild()) {
        collapseAnonymousBlockChild(this, toRenderBlock(child));
    } else if (((prev && prev->isAnonymousBlock()) || (next && next->isAnonymousBlock())) && canCollapseAnonymousBlockChild()) {
        RenderBlock* anonymousBlock = toRenderBlock((prev && prev->isAnonymousBlock()) ? prev : next);
        if ((anonymousBlock->previousSibling() || anonymousBlock->nextSibling())
            && (!anonymousBlock->previousSibling() || (anonymousBlock->previousSibling()->style()->styleType() != NOPSEUDO && anonymousBlock->previousSibling()->isFloating() && !anonymousBlock->previousSibling()->previousSibling()))
            && (!anonymousBlock->nextSibling() || (anonymousBlock->nextSibling()->style()->styleType() != NOPSEUDO && anonymousBlock->nextSibling()->isFloating() && !anonymousBlock->nextSibling()->nextSibling()))) {
            collapseAnonymousBlockChild(this, anonymousBlock);
        }
    }

    if (!firstChild()) {
        if (childrenInline())
            deleteLineBoxTree();

        if (!beingDestroyed() && isAnonymousBlockContinuation() && !oldChild->isListMarker()) {
            RenderObject* containingBlockIgnoringAnonymous = containingBlock();
            while (containingBlockIgnoringAnonymous && containingBlockIgnoringAnonymous->isAnonymousBlock())
                containingBlockIgnoringAnonymous = containingBlockIgnoringAnonymous->containingBlock();
            for (RenderObject* curr = this; curr; curr = curr->previousInPreOrder(containingBlockIgnoringAnonymous)) {
                if (curr->virtualContinuation() != this)
                    continue;

                RenderBoxModelObject* nextContinuation = continuation();
                if (curr->isRenderInline())
                    toRenderInline(curr)->setContinuation(nextContinuation);
                else if (curr->isRenderBlock())
                    toRenderBlock(curr)->setContinuation(nextContinuation);
                else
                    ASSERT_NOT_REACHED();

                break;
            }
            setContinuation(0);
            destroy();
        }
    }
}
