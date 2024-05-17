void WebLocalFrameImpl::ReloadImage(const WebNode& web_node) {
  const Node* node = web_node.ConstUnwrap<Node>();
  if (isHTMLImageElement(*node)) {
    const HTMLImageElement& image_element = toHTMLImageElement(*node);
    image_element.ForceReload();
  }
}
