void WebLocalFrameImpl::ReloadImage(const WebNode& web_node) {
  const Node* node = web_node.ConstUnwrap<Node>();
  if (auto* image_element = ToHTMLImageElementOrNull(*node))
    image_element->ForceReload();
}
