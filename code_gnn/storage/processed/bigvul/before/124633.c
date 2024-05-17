void RenderBlock::removePositionedObjects(RenderBlock* o, ContainingBlockState containingBlockState)
{
    TrackedRendererListHashSet* positionedDescendants = positionedObjects();
    if (!positionedDescendants)
        return;

    RenderBox* r;

    TrackedRendererListHashSet::iterator end = positionedDescendants->end();

    Vector<RenderBox*, 16> deadObjects;

    for (TrackedRendererListHashSet::iterator it = positionedDescendants->begin(); it != end; ++it) {
        r = *it;
        if (!o || r->isDescendantOf(o)) {
            if (containingBlockState == NewContainingBlock)
                r->setChildNeedsLayout(MarkOnlyThis);

            RenderObject* p = r->parent();
            while (p && !p->isRenderBlock())
                p = p->parent();
            if (p)
                p->setChildNeedsLayout();

            deadObjects.append(r);
        }
    }

    for (unsigned i = 0; i < deadObjects.size(); i++)
        removePositionedObject(deadObjects.at(i));
}
