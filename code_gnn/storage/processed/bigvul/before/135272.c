bool Document::importContainerNodeChildren(ContainerNode* oldContainerNode, PassRefPtrWillBeRawPtr<ContainerNode> newContainerNode, ExceptionState& exceptionState)
{
    for (Node& oldChild : NodeTraversal::childrenOf(*oldContainerNode)) {
        RefPtrWillBeRawPtr<Node> newChild = importNode(&oldChild, true, exceptionState);
        if (exceptionState.hadException())
            return false;
        newContainerNode->appendChild(newChild.release(), exceptionState);
        if (exceptionState.hadException())
            return false;
    }

    return true;
}
