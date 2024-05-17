PassRefPtr<NodeIterator> Document::createNodeIterator(Node* root, ExceptionState& es)
{
    if (!root) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    return NodeIterator::create(root, NodeFilter::SHOW_ALL, PassRefPtr<NodeFilter>());
}
