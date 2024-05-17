void RenderBlock::addChildIgnoringAnonymousColumnBlocks(RenderObject* newChild, RenderObject* beforeChild)
{
    if (beforeChild && beforeChild->parent() != this) {
        RenderObject* beforeChildContainer = beforeChild->parent();
        while (beforeChildContainer->parent() != this)
            beforeChildContainer = beforeChildContainer->parent();
        ASSERT(beforeChildContainer);

        if (beforeChildContainer->isAnonymous()) {
            RenderObject* beforeChildAnonymousContainer = beforeChildContainer;
            if (beforeChildAnonymousContainer->isAnonymousBlock()
                || beforeChildAnonymousContainer->isRenderFullScreen()
                || beforeChildAnonymousContainer->isRenderFullScreenPlaceholder()
                ) {
                if (newChild->isInline() || newChild->isFloatingOrOutOfFlowPositioned() || beforeChild->parent()->firstChild() != beforeChild)
                    beforeChild->parent()->addChild(newChild, beforeChild);
                else
                    addChild(newChild, beforeChild->parent());
                return;
            }

            ASSERT(beforeChildAnonymousContainer->isTable());
            if (newChild->isTablePart()) {
                beforeChildAnonymousContainer->addChild(newChild, beforeChild);
                return;
            }

            beforeChild = splitAnonymousBoxesAroundChild(beforeChild);

            ASSERT(beforeChild->parent() == this);
            if (beforeChild->parent() != this) {
                beforeChild = beforeChildContainer;
            }
        }
    }

    if (gColumnFlowSplitEnabled && !document().regionBasedColumnsEnabled()) {
        RenderBlockFlow* columnsBlockAncestor = columnsBlockForSpanningElement(newChild);
        if (columnsBlockAncestor) {
            TemporaryChange<bool> columnFlowSplitEnabled(gColumnFlowSplitEnabled, false);
            RenderBlockFlow* newBox = createAnonymousColumnSpanBlock();

            if (columnsBlockAncestor != this && !isRenderFlowThread()) {
                RenderBoxModelObject* oldContinuation = continuation();

                if (!isAnonymousBlock())
                    setContinuation(newBox);

                splitFlow(beforeChild, newBox, newChild, oldContinuation);
                return;
            }

            makeChildrenAnonymousColumnBlocks(beforeChild, newBox, newChild);
            return;
        }
    }

    bool madeBoxesNonInline = false;

    if (childrenInline() && !newChild->isInline() && !newChild->isFloatingOrOutOfFlowPositioned()) {
        makeChildrenNonInline(beforeChild);
        madeBoxesNonInline = true;

        if (beforeChild && beforeChild->parent() != this) {
            beforeChild = beforeChild->parent();
            ASSERT(beforeChild->isAnonymousBlock());
            ASSERT(beforeChild->parent() == this);
        }
    } else if (!childrenInline() && (newChild->isFloatingOrOutOfFlowPositioned() || newChild->isInline())) {
        RenderObject* afterChild = beforeChild ? beforeChild->previousSibling() : lastChild();

        if (afterChild && afterChild->isAnonymousBlock()) {
            afterChild->addChild(newChild);
            return;
        }

        if (newChild->isInline()) {
            RenderBlock* newBox = createAnonymousBlock();
            RenderBox::addChild(newBox, beforeChild);
            newBox->addChild(newChild);
            return;
        }
    }

    RenderBox::addChild(newChild, beforeChild);

    if (madeBoxesNonInline && parent() && isAnonymousBlock() && parent()->isRenderBlock())
        toRenderBlock(parent())->removeLeftoverAnonymousBlock(this);
}
