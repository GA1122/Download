void SpeechRecognitionManagerImpl::OnEnvironmentEstimationComplete(
    int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!SessionExists(session_id))
    return;

  DCHECK_EQ(primary_session_id_, session_id);
  if (SpeechRecognitionEventListener* delegate_listener = GetDelegateListener())
    delegate_listener->OnEnvironmentEstimationComplete(session_id);
  if (SpeechRecognitionEventListener* listener = GetListener(session_id))
    listener->OnEnvironmentEstimationComplete(session_id);
}
