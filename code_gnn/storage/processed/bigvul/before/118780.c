static void updateTreeAfterInsertion(ContainerNode* parent, Node* child)
{
    ASSERT(parent->refCount());
    ASSERT(child->refCount());

    ChildListMutationScope(parent).childAdded(child);

    parent->childrenChanged(false, child->previousSibling(), child->nextSibling(), 1);

    ChildNodeInsertionNotifier(parent).notify(child);

    dispatchChildInsertionEvents(child);
}
