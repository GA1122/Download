bool SpeechRecognitionManagerImpl::SessionExists(int session_id) const {
  return sessions_.find(session_id) != sessions_.end();
}
