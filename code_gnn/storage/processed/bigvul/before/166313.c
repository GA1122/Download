void SpeechRecognitionManagerImpl::AbortAllSessionsForRenderFrame(
    int render_process_id,
    int render_frame_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  for (const auto& session_pair : sessions_) {
    Session* session = session_pair.second.get();
    if (session->context.render_process_id == render_process_id &&
        session->context.render_frame_id == render_frame_id) {
      AbortSession(session->id);
    }
  }
}
