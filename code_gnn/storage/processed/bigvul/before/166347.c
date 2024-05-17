void SpeechRecognitionManagerImpl::ResetCapturingSessionId(
    const Session& session) {
  DCHECK_EQ(primary_session_id_, session.id);
  primary_session_id_ = kSessionIDInvalid;
}
