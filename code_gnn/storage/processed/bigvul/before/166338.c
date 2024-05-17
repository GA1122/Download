void SpeechRecognitionManagerImpl::OnRecognitionEnd(int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!SessionExists(session_id))
    return;

  if (SpeechRecognitionEventListener* delegate_listener = GetDelegateListener())
    delegate_listener->OnRecognitionEnd(session_id);
  if (SpeechRecognitionEventListener* listener = GetListener(session_id))
    listener->OnRecognitionEnd(session_id);
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&SpeechRecognitionManagerImpl::DispatchEvent,
                                weak_factory_.GetWeakPtr(), session_id,
                                EVENT_RECOGNITION_ENDED));
}
