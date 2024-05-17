int Document::nodeAbsIndex(Node *node)
{
    ASSERT(node->document() == this);

    int absIndex = 0;
    for (Node* n = node; n && n != this; n = NodeTraversal::previous(n))
        absIndex++;
    return absIndex;
}
