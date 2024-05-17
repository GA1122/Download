PassRefPtr<HTMLCollection> ContainerNode::children()
{
    return ensureRareData()->ensureNodeLists()->addCacheWithAtomicName<HTMLCollection>(this, NodeChildren);
}
