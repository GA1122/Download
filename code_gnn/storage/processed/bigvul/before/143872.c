void NodeIterator::updateForNodeRemoval(Node& removedNode, NodePointer& referenceNode) const
{
    ASSERT(root()->document() == removedNode.document());

    if (!removedNode.isDescendantOf(root()))
        return;
    bool willRemoveReferenceNode = removedNode == referenceNode.node.get();
    bool willRemoveReferenceNodeAncestor = referenceNode.node && referenceNode.node->isDescendantOf(&removedNode);
    if (!willRemoveReferenceNode && !willRemoveReferenceNodeAncestor)
        return;

    if (referenceNode.isPointerBeforeNode) {
        Node* node = NodeTraversal::next(removedNode, root());
        if (node) {
            while (node && node->isDescendantOf(&removedNode))
                node = NodeTraversal::next(*node, root());
            if (node)
                referenceNode.node = node;
        } else {
            node = NodeTraversal::previous(removedNode, root());
            if (node) {
                if (willRemoveReferenceNodeAncestor) {
                    while (node && node->isDescendantOf(&removedNode))
                        node = NodeTraversal::previous(*node, root());
                }
                if (node) {
                    referenceNode.node = node;
                    referenceNode.isPointerBeforeNode = false;
                }
            }
        }
    } else {
        Node* node = NodeTraversal::previous(removedNode, root());
        if (node) {
            if (willRemoveReferenceNodeAncestor) {
                while (node && node->isDescendantOf(&removedNode))
                    node = NodeTraversal::previous(*node, root());
            }
            if (node)
                referenceNode.node = node;
        } else {
            node = NodeTraversal::next(removedNode, root());
            if (willRemoveReferenceNodeAncestor) {
                while (node && node->isDescendantOf(&removedNode))
                    node = NodeTraversal::previous(*node, root());
            }
            if (node)
                referenceNode.node = node;
        }
    }
}
