void Document::moveNodeIteratorsToNewDocument(Node& node, Document& newDocument)
{
    WillBeHeapHashSet<RawPtrWillBeWeakMember<NodeIterator>> nodeIteratorsList = m_nodeIterators;
    for (NodeIterator* ni : nodeIteratorsList) {
        if (ni->root() == node) {
            detachNodeIterator(ni);
            newDocument.attachNodeIterator(ni);
        }
    }
}
