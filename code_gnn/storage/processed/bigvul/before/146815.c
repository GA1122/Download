void Document::RemoveAllEventListeners() {
  ContainerNode::RemoveAllEventListeners();

  if (LocalDOMWindow* dom_window = this->domWindow())
    dom_window->RemoveAllEventListeners();
}
