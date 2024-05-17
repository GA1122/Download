String dispatchBeforeTextInsertedEvent(const String& text,
                                       const VisibleSelection& selection) {
  String newText = text;
  if (Node* startNode = selection.start().computeContainerNode()) {
    if (rootEditableElement(*startNode)) {
      BeforeTextInsertedEvent* evt = BeforeTextInsertedEvent::create(text);
      rootEditableElement(*startNode)->dispatchEvent(evt);
      newText = evt->text();
    }
  }
  return newText;
}
