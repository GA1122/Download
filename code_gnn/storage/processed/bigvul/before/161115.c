void MediaStreamManager::DoNativeLogCallbackRegistration(
    int renderer_host_id,
    const base::Callback<void(const std::string&)>& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  log_callbacks_[renderer_host_id] = callback;
}
