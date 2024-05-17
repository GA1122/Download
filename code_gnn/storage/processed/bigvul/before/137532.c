blink::WebPlugin* GetPlugin(const blink::WebFrame* frame) {
  return frame->document().isPluginDocument()
             ? frame->document().to<blink::WebPluginDocument>().plugin()
             : NULL;
}
