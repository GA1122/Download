void ContainerNode::removeDetachedChildrenInContainer(ContainerNode& container)
{
    Node* head = nullptr;
    Node* tail = nullptr;

    addChildNodesToDeletionQueue(head, tail, container);

    Node* n;
    Node* next;
    while (head) {
        n = head;
        ASSERT_WITH_SECURITY_IMPLICATION(n->m_deletionHasBegun);

        next = n->nextSibling();
        n->setNextSibling(nullptr);

        head = next;
        if (!next)
            tail = nullptr;

        if (n->hasChildren())
            addChildNodesToDeletionQueue(head, tail, toContainerNode(*n));

        delete n;
    }
}
