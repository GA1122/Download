TreeWalker* Document::createTreeWalker(Node* root, unsigned whatToShow, NodeFilter* filter)
{
    DCHECK(root);
    return TreeWalker::create(root, whatToShow, filter);
}
