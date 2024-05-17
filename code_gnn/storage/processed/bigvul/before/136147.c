PassRefPtrWillBeRawPtr<TreeWalker> Document::createTreeWalker(Node* root, unsigned whatToShow, PassRefPtrWillBeRawPtr<NodeFilter> filter, ExceptionState& exceptionState)
{
    return TreeWalker::create(root, whatToShow, filter);
}
