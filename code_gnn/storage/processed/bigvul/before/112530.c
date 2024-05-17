void Document::removeFocusedNodeOfSubtree(Node* node, bool amongChildrenOnly)
{
    if (!m_focusedNode || this->inPageCache())  
        return;

    Node* focusedNode = node->treeScope()->focusedNode();
    if (!focusedNode)
        return;

    bool nodeInSubtree = false;
    if (amongChildrenOnly)
        nodeInSubtree = focusedNode->isDescendantOf(node);
    else
        nodeInSubtree = (focusedNode == node) || focusedNode->isDescendantOf(node);
    
    if (nodeInSubtree)
        document()->focusedNodeRemoved();
}
