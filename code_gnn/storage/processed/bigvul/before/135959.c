void ContainerNode::parserTakeAllChildrenFrom(ContainerNode& oldParent)
{
    while (RefPtrWillBeRawPtr<Node> child = oldParent.firstChild()) {
        oldParent.parserRemoveChild(*child);
        parserAppendChild(child.get());
    }
}
