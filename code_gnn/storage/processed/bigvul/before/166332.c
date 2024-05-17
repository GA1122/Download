void SpeechRecognitionManagerImpl::MediaRequestPermissionCallback(
    int session_id,
    const MediaStreamDevices& devices,
    std::unique_ptr<MediaStreamUIProxy> stream_ui) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  auto iter = sessions_.find(session_id);
  if (iter == sessions_.end())
    return;

  bool is_allowed = !devices.empty();
  if (is_allowed) {
    iter->second->context.devices = devices;

    iter->second->ui = std::move(stream_ui);
  }

  iter->second->context.label.clear();

  RecognitionAllowedCallback(iter->first, false, is_allowed);
}
