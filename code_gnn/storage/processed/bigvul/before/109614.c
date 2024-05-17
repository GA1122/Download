PassRefPtr<NodeIterator> Document::createNodeIterator(Node* root, unsigned whatToShow, PassRefPtr<NodeFilter> filter, bool expandEntityReferences, ExceptionState& es)
{
    if (!root) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    UNUSED_PARAM(expandEntityReferences);
    return NodeIterator::create(root, whatToShow, filter);
}
