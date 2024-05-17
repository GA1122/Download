SpeechRecognitionManagerImpl::GetSessionConfig(int session_id) const {
  return GetSession(session_id)->config;
}
