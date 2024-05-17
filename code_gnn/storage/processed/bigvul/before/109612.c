PassRefPtr<NodeIterator> Document::createNodeIterator(Node* root, unsigned whatToShow, ExceptionState& es)
{
    if (!root) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    return NodeIterator::create(root, whatToShow, PassRefPtr<NodeFilter>());
}
