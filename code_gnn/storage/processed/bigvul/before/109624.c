PassRefPtr<TreeWalker> Document::createTreeWalker(Node* root, unsigned whatToShow, PassRefPtr<NodeFilter> filter, ExceptionState& es)
{
    if (!root) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    return TreeWalker::create(root, whatToShow, filter);
}
