bool PrintingNodeOrPdfFrame(const blink::WebFrame* frame,
                            const blink::WebNode& node) {
  if (!node.isNull())
    return true;
  blink::WebPlugin* plugin = GetPlugin(frame);
  return plugin && plugin->supportsPaginatedPrint();
}
