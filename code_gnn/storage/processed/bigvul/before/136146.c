PassRefPtrWillBeRawPtr<NodeIterator> Document::createNodeIterator(Node* root, unsigned whatToShow, PassRefPtrWillBeRawPtr<NodeFilter> filter, ExceptionState& exceptionState)
{
    return NodeIterator::create(root, whatToShow, filter);
}
