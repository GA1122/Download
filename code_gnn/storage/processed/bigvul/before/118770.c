Element* ContainerNode::lastElementChild() const
{
    Node* n = lastChild();
    while (n && !n->isElementNode())
        n = n->previousSibling();
    return toElement(n);
}
