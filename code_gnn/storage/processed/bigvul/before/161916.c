void PrintRenderFrameHelper::PrintPreviewContext::InitWithNode(
    const blink::WebNode& web_node) {
  DCHECK(!web_node.IsNull());
  DCHECK(web_node.GetDocument().GetFrame());
  DCHECK(!IsRendering());
  state_ = INITIALIZED;
  source_frame_.Reset(web_node.GetDocument().GetFrame());
  source_node_ = web_node;
  CalculateIsModifiable();
}
