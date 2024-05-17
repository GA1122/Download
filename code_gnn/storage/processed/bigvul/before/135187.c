PassRefPtrWillBeRawPtr<NodeIterator> Document::createNodeIterator(Node* root, unsigned whatToShow, PassRefPtrWillBeRawPtr<NodeFilter> filter)
{
    ASSERT(root);
    return NodeIterator::create(root, whatToShow, filter);
}
