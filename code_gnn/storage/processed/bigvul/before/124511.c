void RenderBlock::clearPercentHeightDescendantsFrom(RenderBox* parent)
{
    ASSERT(gPercentHeightContainerMap);
    for (RenderObject* curr = parent->firstChild(); curr; curr = curr->nextInPreOrder(parent)) {
        if (!curr->isBox())
            continue;

        RenderBox* box = toRenderBox(curr);
        if (!hasPercentHeightDescendant(box))
            continue;

        removePercentHeightDescendant(box);
    }
}
