Node* EventTargetNodeForDocument(Document* doc) {
  if (!doc)
    return nullptr;
  Node* node = doc->FocusedElement();
  if (!node && doc->IsPluginDocument()) {
    PluginDocument* plugin_document = ToPluginDocument(doc);
    node = plugin_document->PluginNode();
  }
  if (!node && doc->IsHTMLDocument())
    node = doc->body();
  if (!node)
    node = doc->documentElement();
  return node;
}
