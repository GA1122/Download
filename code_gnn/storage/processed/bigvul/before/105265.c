static void setHasDirAutoFlagRecursively(Node* firstNode, bool flag, Node* lastNode = 0)
{
    firstNode->setSelfOrAncestorHasDirAutoAttribute(flag);

    Node* node = firstNode->firstChild();

    while (node) {
        if (node->selfOrAncestorHasDirAutoAttribute() == flag)
            return;

        if (elementAffectsDirectionality(node)) {
            if (node == lastNode)
                return;
            node = node->traverseNextSibling(firstNode);
            continue;
        }
        node->setSelfOrAncestorHasDirAutoAttribute(flag);
        if (node == lastNode)
            return;
        node = node->traverseNextNode(firstNode);
    }
}
