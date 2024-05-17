void ContainerNode::parserTakeAllChildrenFrom(ContainerNode* oldParent)
{
    while (RefPtr<Node> child = oldParent->firstChild()) {
        oldParent->parserRemoveChild(child.get());
        treeScope().adoptIfNeeded(child.get());
        parserAppendChild(child.get());
    }
}
