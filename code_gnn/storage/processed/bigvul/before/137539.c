void PrintWebViewHelper::PrintPreviewContext::InitWithNode(
    const blink::WebNode& web_node) {
  DCHECK(!web_node.isNull());
  DCHECK(web_node.document().frame());
  DCHECK(!IsRendering());
  state_ = INITIALIZED;
  source_frame_.Reset(web_node.document().frame());
  source_node_ = web_node;
}
