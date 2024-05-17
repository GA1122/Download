void ContainerNode::childrenChanged(const ChildrenChange& change)
{
    document().incDOMTreeVersion();
    if (!change.byParser && change.type != TextChanged)
        document().updateRangesAfterChildrenChanged(this);
    invalidateNodeListCachesInAncestors();
    if (change.isChildInsertion() && !childNeedsStyleRecalc()) {
        setChildNeedsStyleRecalc();
        markAncestorsWithChildNeedsStyleRecalc();
    }
}
