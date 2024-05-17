void ContainerNode::detach(const AttachContext& context)
{
    detachChildren(context);
    setChildNeedsStyleRecalc();
    Node::detach(context);
}
