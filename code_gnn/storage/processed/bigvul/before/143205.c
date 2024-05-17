bool Document::importContainerNodeChildren(ContainerNode* oldContainerNode, ContainerNode* newContainerNode, ExceptionState& exceptionState)
{
    for (Node& oldChild : NodeTraversal::childrenOf(*oldContainerNode)) {
        Node* newChild = importNode(&oldChild, true, exceptionState);
        if (exceptionState.hadException())
            return false;
        newContainerNode->appendChild(newChild, exceptionState);
        if (exceptionState.hadException())
            return false;
    }

    return true;
}
