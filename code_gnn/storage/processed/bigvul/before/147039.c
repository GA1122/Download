int WebLocalFrameImpl::PrintBegin(const WebPrintParams& print_params,
                                  const WebNode& constrain_to_node) {
  DCHECK(!GetFrame()->GetDocument()->IsFrameSet());
  WebPluginContainerImpl* plugin_container = nullptr;
  if (constrain_to_node.IsNull()) {
    plugin_container = GetFrame()->GetWebPluginContainer();
  } else {
    plugin_container =
        ToWebPluginContainerImpl(constrain_to_node.PluginContainer());
  }

  if (plugin_container && plugin_container->SupportsPaginatedPrint()) {
    print_context_ = new ChromePluginPrintContext(GetFrame(), plugin_container,
                                                  print_params);
  } else {
    print_context_ = new ChromePrintContext(GetFrame());
  }

  FloatSize size(static_cast<float>(print_params.print_content_area.width),
                 static_cast<float>(print_params.print_content_area.height));
  print_context_->BeginPrintMode(size.Width(), size.Height());
  print_context_->ComputePageRects(size);

  return static_cast<int>(print_context_->PageCount());
}
