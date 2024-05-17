NodeIterator* Document::createNodeIterator(Node* root, unsigned whatToShow, NodeFilter* filter)
{
    DCHECK(root);
    return NodeIterator::create(root, whatToShow, filter);
}
