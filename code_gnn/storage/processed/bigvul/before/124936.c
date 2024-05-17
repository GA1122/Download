void RenderBox::removeFloatingOrPositionedChildFromBlockLists()
{
    ASSERT(isFloatingOrOutOfFlowPositioned());

    if (documentBeingDestroyed())
        return;

    if (isFloating()) {
        RenderBlockFlow* parentBlockFlow = 0;
        for (RenderObject* curr = parent(); curr && !curr->isRenderView(); curr = curr->parent()) {
            if (curr->isRenderBlockFlow()) {
                RenderBlockFlow* currBlockFlow = toRenderBlockFlow(curr);
                if (!parentBlockFlow || currBlockFlow->containsFloat(this))
                    parentBlockFlow = currBlockFlow;
            }
        }

        if (parentBlockFlow) {
            parentBlockFlow->markSiblingsWithFloatsForLayout(this);
            parentBlockFlow->markAllDescendantsWithFloatsForLayout(this, false);
        }
    }

    if (isOutOfFlowPositioned())
        RenderBlock::removePositionedObject(this);
}
