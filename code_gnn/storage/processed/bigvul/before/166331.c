SpeechRecognitionManagerImpl::GetSessionState(int session_id) const {
  Session* session = GetSession(session_id);
  if (!session->recognizer.get() || !session->recognizer->IsActive())
    return SESSION_STATE_IDLE;
  if (session->recognizer->IsCapturingAudio())
    return SESSION_STATE_CAPTURING_AUDIO;
  return SESSION_STATE_WAITING_FOR_RESULT;
}
