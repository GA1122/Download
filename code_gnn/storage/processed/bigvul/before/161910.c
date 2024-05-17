blink::WebPlugin* GetPlugin(const blink::WebLocalFrame* frame) {
  return frame->GetDocument().IsPluginDocument()
             ? frame->GetDocument().To<blink::WebPluginDocument>().Plugin()
             : nullptr;
}
