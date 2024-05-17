bool PrintingNodeOrPdfFrame(const WebFrame* frame, const WebNode& node) {
  if (!node.isNull())
    return true;
  if (!frame->document().isPluginDocument())
    return false;
  WebPlugin* plugin = frame->document().to<WebPluginDocument>().plugin();
  return plugin && plugin->supportsPaginatedPrint();
}
