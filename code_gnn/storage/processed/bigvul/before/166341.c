void SpeechRecognitionManagerImpl::OnRecognitionStart(int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!SessionExists(session_id))
    return;

  auto iter = sessions_.find(session_id);
  if (iter->second->ui) {
    iter->second->ui->OnStarted(base::OnceClosure(),
                                MediaStreamUIProxy::WindowIdCallback());
  }

  DCHECK_EQ(primary_session_id_, session_id);
  if (SpeechRecognitionEventListener* delegate_listener = GetDelegateListener())
    delegate_listener->OnRecognitionStart(session_id);
  if (SpeechRecognitionEventListener* listener = GetListener(session_id))
    listener->OnRecognitionStart(session_id);
}
