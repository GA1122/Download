FrameFetchContext::CreateWebSocketHandshakeThrottle() {
  if (IsDetached()) {
    return nullptr;
  }
  if (!GetFrame())
    return nullptr;
  return WebFrame::FromFrame(GetFrame())
      ->ToWebLocalFrame()
      ->Client()
      ->CreateWebSocketHandshakeThrottle();
}
