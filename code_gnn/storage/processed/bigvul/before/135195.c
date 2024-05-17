PassRefPtrWillBeRawPtr<TreeWalker> Document::createTreeWalker(Node* root, unsigned whatToShow, PassRefPtrWillBeRawPtr<NodeFilter> filter)
{
    ASSERT(root);
    return TreeWalker::create(root, whatToShow, filter);
}
