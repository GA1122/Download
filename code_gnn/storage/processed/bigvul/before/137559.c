void PrintWebViewHelper::PrintNode(const blink::WebNode& node) {
  if (node.isNull() || !node.document().frame()) {
    return;
  }

  if (print_node_in_progress_) {
    return;
  }

  print_node_in_progress_ = true;

  if (!g_is_preview_enabled_) {
    blink::WebNode duplicate_node(node);
    Print(duplicate_node.document().frame(), duplicate_node, false);
  } else {
    print_preview_context_.InitWithNode(node);
    RequestPrintPreview(PRINT_PREVIEW_USER_INITIATED_CONTEXT_NODE);
  }

  print_node_in_progress_ = false;
}
