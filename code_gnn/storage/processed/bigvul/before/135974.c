void ContainerNode::updateTreeAfterInsertion(Node& child)
{
#if !ENABLE(OILPAN)
    ASSERT(refCount());
    ASSERT(child.refCount());
#endif

    ChildListMutationScope(*this).childAdded(child);

    notifyNodeInserted(child);

    dispatchChildInsertionEvents(child);
}
