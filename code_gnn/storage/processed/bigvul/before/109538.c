bool PrintingNodeOrPdfFrame(const WebKit::WebFrame* frame,
                            const WebKit::WebNode& node) {
  if (!node.isNull())
    return true;
  if (!frame->document().isPluginDocument())
    return false;
  WebKit::WebPlugin* plugin =
      frame->document().to<WebKit::WebPluginDocument>().plugin();
  return plugin && plugin->supportsPaginatedPrint();
}
