void MediaControlPanelElement::defaultEventHandler(Event* event) {
  if (event->type() == EventTypeNames::click) {
    event->setDefaultHandled();
    return;
  }
  HTMLDivElement::defaultEventHandler(event);
}
