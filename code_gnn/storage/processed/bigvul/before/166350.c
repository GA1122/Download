void SpeechRecognitionManagerImpl::SessionDelete(Session* session) {
  DCHECK(session->recognizer.get() == nullptr ||
         !session->recognizer->IsActive());
  if (primary_session_id_ == session->id)
    primary_session_id_ = kSessionIDInvalid;
  if (!session->context.label.empty())
    media_stream_manager_->CancelRequest(session->context.label);
  sessions_.erase(session->id);
}
