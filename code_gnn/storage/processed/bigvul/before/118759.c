static void collectChildrenAndRemoveFromOldParent(Node* node, NodeVector& nodes, ExceptionState& es)
{
    if (node->nodeType() != Node::DOCUMENT_FRAGMENT_NODE) {
        nodes.append(node);
        if (ContainerNode* oldParent = node->parentNode())
            oldParent->removeChild(node, es);
        return;
    }
    getChildNodes(node, nodes);
    toContainerNode(node)->removeChildren();
}
