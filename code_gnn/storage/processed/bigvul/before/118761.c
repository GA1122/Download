void ContainerNode::detach(const AttachContext& context)
{
    detachChildren(context);
    clearChildNeedsStyleRecalc();
    Node::detach(context);
}
