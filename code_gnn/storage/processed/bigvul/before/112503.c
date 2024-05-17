Node* Document::nodeWithAbsIndex(int absIndex)
{
    Node* n = this;
    for (int i = 0; n && (i < absIndex); i++)
        n = NodeTraversal::next(n);
    return n;
}
