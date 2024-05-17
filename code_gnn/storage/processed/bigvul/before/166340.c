void SpeechRecognitionManagerImpl::OnRecognitionResults(
    int session_id,
    const std::vector<blink::mojom::SpeechRecognitionResultPtr>& results) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!SessionExists(session_id))
    return;

  if (SpeechRecognitionEventListener* delegate_listener = GetDelegateListener())
    delegate_listener->OnRecognitionResults(session_id, results);
  if (SpeechRecognitionEventListener* listener = GetListener(session_id))
    listener->OnRecognitionResults(session_id, results);
}
