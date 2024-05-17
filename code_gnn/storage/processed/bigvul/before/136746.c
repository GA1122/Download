void LocalDOMWindow::DocumentWasClosed() {
  DispatchWindowLoadEvent();
  EnqueuePageshowEvent(kPageshowEventNotPersisted);
  if (pending_state_object_)
    EnqueuePopstateEvent(std::move(pending_state_object_));
}
