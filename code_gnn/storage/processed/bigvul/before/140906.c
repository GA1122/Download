void PresentationConnection::didChangeState(
    WebPresentationConnectionState state,
    bool shouldDispatchEvent) {
  if (m_state == state)
    return;

  m_state = state;

  if (!shouldDispatchEvent)
    return;

  switch (m_state) {
    case WebPresentationConnectionState::Connecting:
      NOTREACHED();
      return;
    case WebPresentationConnectionState::Connected:
      dispatchStateChangeEvent(Event::create(EventTypeNames::connect));
      return;
    case WebPresentationConnectionState::Terminated:
      dispatchStateChangeEvent(Event::create(EventTypeNames::terminate));
      return;
    case WebPresentationConnectionState::Closed:
      NOTREACHED();
      return;
  }
  NOTREACHED();
}
