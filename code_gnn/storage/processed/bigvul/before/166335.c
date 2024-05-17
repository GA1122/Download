void SpeechRecognitionManagerImpl::OnAudioLevelsChange(
    int session_id, float volume, float noise_volume) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!SessionExists(session_id))
    return;

  if (SpeechRecognitionEventListener* delegate_listener = GetDelegateListener())
    delegate_listener->OnAudioLevelsChange(session_id, volume, noise_volume);
  if (SpeechRecognitionEventListener* listener = GetListener(session_id))
    listener->OnAudioLevelsChange(session_id, volume, noise_volume);
}
