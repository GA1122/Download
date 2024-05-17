unsigned ContainerNode::countChildren() const
{
    unsigned count = 0;
    Node* n;
    for (n = firstChild(); n; n = n->nextSibling())
        count++;
    return count;
}
