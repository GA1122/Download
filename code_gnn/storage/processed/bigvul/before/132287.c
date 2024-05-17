blink::WebPluginPlaceholder* RenderFrameImpl::createPluginPlaceholder(
    blink::WebLocalFrame* frame,
    const blink::WebPluginParams& params) {
  DCHECK_EQ(frame_, frame);
  return GetContentClient()
      ->renderer()
      ->CreatePluginPlaceholder(this, frame, params)
      .release();
}
