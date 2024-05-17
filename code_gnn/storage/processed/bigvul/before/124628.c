void RenderBlock::removeFromTrackedRendererMaps(RenderBox* descendant, TrackedDescendantsMap*& descendantsMap, TrackedContainerMap*& containerMap)
{
    if (!descendantsMap)
        return;

    OwnPtr<HashSet<RenderBlock*> > containerSet = containerMap->take(descendant);
    if (!containerSet)
        return;

    HashSet<RenderBlock*>::iterator end = containerSet->end();
    for (HashSet<RenderBlock*>::iterator it = containerSet->begin(); it != end; ++it) {
        RenderBlock* container = *it;


        TrackedDescendantsMap::iterator descendantsMapIterator = descendantsMap->find(container);
        ASSERT(descendantsMapIterator != descendantsMap->end());
        if (descendantsMapIterator == descendantsMap->end())
            continue;
        TrackedRendererListHashSet* descendantSet = descendantsMapIterator->value.get();
        ASSERT(descendantSet->contains(descendant));
        descendantSet->remove(descendant);
        if (descendantSet->isEmpty())
            descendantsMap->remove(descendantsMapIterator);
    }
}
