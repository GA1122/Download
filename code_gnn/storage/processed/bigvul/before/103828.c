void RenderView::OnScrollFocusedEditableNodeIntoView() {
  WebKit::WebNode node = GetFocusedNode();
  if (!node.isNull()) {
    if (IsEditableNode(node))
      webview()->scrollFocusedNodeIntoView();
  }
}
