PassRefPtr<TreeWalker> Document::createTreeWalker(Node* root, unsigned whatToShow, PassRefPtr<NodeFilter> filter, bool expandEntityReferences, ExceptionState& es)
{
    UNUSED_PARAM(expandEntityReferences);
    if (!root) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    return TreeWalker::create(root, whatToShow, filter);
}
