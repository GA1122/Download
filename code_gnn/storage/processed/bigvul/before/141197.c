void Document::RemoveAllEventListeners() {
  ContainerNode::RemoveAllEventListeners();

  if (LocalDOMWindow* dom_window = domWindow())
    dom_window->RemoveAllEventListeners();
}
