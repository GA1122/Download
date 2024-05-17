void RenderViewImpl::OnUndoScrollFocusedEditableNodeIntoRect() {
  const WebNode node = GetFocusedNode();
  if (!node.isNull() && IsEditableNode(node))
    webview()->restoreScrollAndScaleState();
}
