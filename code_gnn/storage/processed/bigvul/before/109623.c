PassRefPtr<TreeWalker> Document::createTreeWalker(Node* root, unsigned whatToShow, ExceptionState& es)
{
    if (!root) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    return TreeWalker::create(root, whatToShow, PassRefPtr<NodeFilter>());
}
