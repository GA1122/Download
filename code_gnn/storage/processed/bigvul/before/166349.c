void SpeechRecognitionManagerImpl::SessionAbort(const Session& session) {
  if (primary_session_id_ == session.id)
    primary_session_id_ = kSessionIDInvalid;
  DCHECK(session.recognizer.get());
  session.recognizer->AbortRecognition();
}
