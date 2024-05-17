RenderBlock* RenderBlock::blockBeforeWithinSelectionRoot(LayoutSize& offset) const
{
    if (isSelectionRoot())
        return 0;

    const RenderObject* object = this;
    RenderObject* sibling;
    do {
        sibling = object->previousSibling();
        while (sibling && (!sibling->isRenderBlock() || toRenderBlock(sibling)->isSelectionRoot()))
            sibling = sibling->previousSibling();

        offset -= LayoutSize(toRenderBlock(object)->logicalLeft(), toRenderBlock(object)->logicalTop());
        object = object->parent();
    } while (!sibling && object && object->isRenderBlock() && !toRenderBlock(object)->isSelectionRoot());

    if (!sibling)
        return 0;

    RenderBlock* beforeBlock = toRenderBlock(sibling);

    offset += LayoutSize(beforeBlock->logicalLeft(), beforeBlock->logicalTop());

    RenderObject* child = beforeBlock->lastChild();
    while (child && child->isRenderBlock()) {
        beforeBlock = toRenderBlock(child);
        offset += LayoutSize(beforeBlock->logicalLeft(), beforeBlock->logicalTop());
        child = beforeBlock->lastChild();
    }
    return beforeBlock;
}
