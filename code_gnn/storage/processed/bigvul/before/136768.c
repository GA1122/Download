void LocalDOMWindow::RemovedEventListener(
    const AtomicString& event_type,
    const RegisteredEventListener& registered_listener) {
  DOMWindow::RemovedEventListener(event_type, registered_listener);
  if (GetFrame() && GetFrame()->GetPage())
    GetFrame()->GetPage()->GetEventHandlerRegistry().DidRemoveEventHandler(
        *this, event_type, registered_listener.Options());

  for (auto& it : event_listener_observers_) {
    it->DidRemoveEventListener(this, event_type);
  }

  if (event_type == EventTypeNames::unload) {
    UntrackUnloadEventListener(this);
  } else if (event_type == EventTypeNames::beforeunload) {
    UntrackBeforeUnloadEventListener(this);
  }
}
