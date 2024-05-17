void Document::updateStyleAndLayoutIgnorePendingStylesheetsForNode(Node* node)
{
    DCHECK(node);
    if (!node->inActiveDocument())
        return;
    updateStyleAndLayoutIgnorePendingStylesheets();
}
