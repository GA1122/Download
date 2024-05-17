void SpeechRecognitionManagerImpl::OnSoundEnd(int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!SessionExists(session_id))
    return;

  if (SpeechRecognitionEventListener* delegate_listener = GetDelegateListener())
    delegate_listener->OnSoundEnd(session_id);
  if (SpeechRecognitionEventListener* listener = GetListener(session_id))
    listener->OnSoundEnd(session_id);
}
