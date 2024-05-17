void PrintRenderFrameHelper::PrintNode(const blink::WebNode& node) {
  if (node.IsNull() || !node.GetDocument().GetFrame()) {
    return;
  }

  if (print_node_in_progress_) {
    return;
  }

  print_node_in_progress_ = true;

  if (g_is_preview_enabled) {
#if BUILDFLAG(ENABLE_PRINT_PREVIEW)
    print_preview_context_.InitWithNode(node);
    RequestPrintPreview(PRINT_PREVIEW_USER_INITIATED_CONTEXT_NODE);
#endif
  } else {
#if BUILDFLAG(ENABLE_BASIC_PRINTING)
    blink::WebNode duplicate_node(node);

    auto self = weak_ptr_factory_.GetWeakPtr();
    Print(duplicate_node.GetDocument().GetFrame(), duplicate_node,
          false  );
    if (!self)
      return;
#endif
  }

  print_node_in_progress_ = false;
}
