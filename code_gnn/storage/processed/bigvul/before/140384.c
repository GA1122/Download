bool canAppendNewLineFeedToSelection(const VisibleSelection& selection) {
  Element* element = selection.rootEditableElement();
  if (!element)
    return false;

  BeforeTextInsertedEvent* event =
      BeforeTextInsertedEvent::create(String("\n"));
  element->dispatchEvent(event);
  return event->text().length();
}
