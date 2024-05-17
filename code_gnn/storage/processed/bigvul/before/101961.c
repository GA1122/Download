void PrintWebViewHelper::PrintPreviewContext::InitWithNode(
    const WebKit::WebNode& web_node) {
  DCHECK(!web_node.isNull());
  state_ = INITIALIZED;
  frame_ = web_node.document().frame();
  node_.reset(new WebNode(web_node));
}
