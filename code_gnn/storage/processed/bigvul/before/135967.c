void ContainerNode::removeDetachedChildren()
{
    ASSERT(!connectedSubframeCount());
    ASSERT(needsAttach());
    removeDetachedChildrenInContainer(*this);
}
