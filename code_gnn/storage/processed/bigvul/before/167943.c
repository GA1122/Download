WebPluginContainerImpl* LocalFrame::GetWebPluginContainer(Node* node) const {
  if (GetDocument() && GetDocument()->IsPluginDocument()) {
    return ToPluginDocument(GetDocument())->GetPluginView();
  }
  if (!node) {
    DCHECK(GetDocument());
    node = GetDocument()->FocusedElement();
  }

  if (node) {
    return node->GetWebPluginContainer();
  }
  return nullptr;
}
