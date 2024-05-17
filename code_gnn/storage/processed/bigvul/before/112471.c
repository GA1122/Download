void Document::getFocusableNodes(Vector<RefPtr<Node> >& nodes)
{
    updateLayout();

    for (Node* node = firstChild(); node; node = NodeTraversal::next(node)) {
        if (node->isFocusable())
            nodes.append(node);
    }
}
