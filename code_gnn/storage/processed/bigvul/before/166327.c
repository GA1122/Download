int SpeechRecognitionManagerImpl::GetNextSessionID() {
  ++last_session_id_;
  if (last_session_id_ <= 0)
    last_session_id_ = 1;
  return last_session_id_;
}
