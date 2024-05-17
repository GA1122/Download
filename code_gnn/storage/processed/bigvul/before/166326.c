SpeechRecognitionEventListener* SpeechRecognitionManagerImpl::GetListener(
    int session_id) const {
  Session* session = GetSession(session_id);
  if (session->config.event_listener)
    return session->config.event_listener.get();
  return nullptr;
}
