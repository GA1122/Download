static void DispatchEditableContentChangedEvents(Element* start_root,
                                                 Element* end_root) {
  if (start_root)
    start_root->DispatchEvent(
        Event::Create(EventTypeNames::webkitEditableContentChanged));
  if (end_root && end_root != start_root)
    end_root->DispatchEvent(
        Event::Create(EventTypeNames::webkitEditableContentChanged));
}
