void LocalDOMWindow::RemoveAllEventListeners() {
  EventTarget::RemoveAllEventListeners();

  for (auto& it : event_listener_observers_) {
    it->DidRemoveAllEventListeners(this);
  }

  if (GetFrame() && GetFrame()->GetPage())
    GetFrame()->GetPage()->GetEventHandlerRegistry().DidRemoveAllEventHandlers(
        *this);

  UntrackAllUnloadEventListeners(this);
  UntrackAllBeforeUnloadEventListeners(this);
}
