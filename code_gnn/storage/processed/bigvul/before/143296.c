void Document::updateStyleAndLayoutTreeForNode(const Node* node)
{
    DCHECK(node);
    if (!needsLayoutTreeUpdateForNode(*node))
        return;
    updateStyleAndLayoutTree();
}
