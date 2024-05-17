void PrintWebViewHelper::OnPrintNodeUnderContextMenu() {
  const WebNode& context_menu_node = render_view()->GetContextMenuNode();
  PrintNode(context_menu_node);
}
