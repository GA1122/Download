Element* ContainerNode::firstElementChild() const
{
    return ElementTraversal::firstWithin(this);
}
