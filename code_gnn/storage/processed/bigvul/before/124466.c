int SimplifyMarkupCommand::pruneSubsequentAncestorsToRemove(Vector<RefPtr<Node> >& nodesToRemove, size_t startNodeIndex)
{
    size_t pastLastNodeToRemove = startNodeIndex + 1;
    for (; pastLastNodeToRemove < nodesToRemove.size(); ++pastLastNodeToRemove) {
        if (nodesToRemove[pastLastNodeToRemove - 1]->parentNode() != nodesToRemove[pastLastNodeToRemove])
            break;
        ASSERT(nodesToRemove[pastLastNodeToRemove]->firstChild() == nodesToRemove[pastLastNodeToRemove]->lastChild());
    }

    Node* highestAncestorToRemove = nodesToRemove[pastLastNodeToRemove - 1].get();
    RefPtr<ContainerNode> parent = highestAncestorToRemove->parentNode();
    if (!parent)  
        return -1;

    if (pastLastNodeToRemove == startNodeIndex + 1)
        return 0;

    removeNode(nodesToRemove[startNodeIndex], AssumeContentIsAlwaysEditable);
    insertNodeBefore(nodesToRemove[startNodeIndex], highestAncestorToRemove, AssumeContentIsAlwaysEditable);
    removeNode(highestAncestorToRemove, AssumeContentIsAlwaysEditable);

    return pastLastNodeToRemove - startNodeIndex - 1;
}
