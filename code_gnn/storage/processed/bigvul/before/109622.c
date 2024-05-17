PassRefPtr<TreeWalker> Document::createTreeWalker(Node* root, ExceptionState& es)
{
    if (!root) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    return TreeWalker::create(root, NodeFilter::SHOW_ALL, PassRefPtr<NodeFilter>());
}
