bool ContainerNode::isChildTypeAllowed(const Node& child) const
{
    if (!child.isDocumentFragment())
        return childTypeAllowed(child.nodeType());

    for (Node* node = toDocumentFragment(child).firstChild(); node; node = node->nextSibling()) {
        if (!childTypeAllowed(node->nodeType()))
            return false;
    }
    return true;
}
