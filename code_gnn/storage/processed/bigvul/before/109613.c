PassRefPtr<NodeIterator> Document::createNodeIterator(Node* root, unsigned whatToShow, PassRefPtr<NodeFilter> filter, ExceptionState& es)
{
    if (!root) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    return NodeIterator::create(root, whatToShow, filter);
}
