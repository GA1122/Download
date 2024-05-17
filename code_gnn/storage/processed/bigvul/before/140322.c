static void dispatchEditableContentChangedEvents(Element* startRoot,
                                                 Element* endRoot) {
  if (startRoot)
    startRoot->dispatchEvent(
        Event::create(EventTypeNames::webkitEditableContentChanged));
  if (endRoot && endRoot != startRoot)
    endRoot->dispatchEvent(
        Event::create(EventTypeNames::webkitEditableContentChanged));
}
