void Document::UpdateStyleAndLayoutIgnorePendingStylesheetsForNode(Node* node) {
  DCHECK(node);
  if (!node->InActiveDocument())
    return;
  UpdateStyleAndLayoutIgnorePendingStylesheets();
}
