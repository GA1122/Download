void MediaStreamManager::RegisterNativeLogCallback(
    int renderer_host_id,
    const base::Callback<void(const std::string&)>& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  MediaStreamManager* msm = g_media_stream_manager_tls_ptr.Pointer()->Get();
  if (!msm) {
    DLOG(ERROR) << "No MediaStreamManager on the IO thread.";
    return;
  }

  msm->DoNativeLogCallbackRegistration(renderer_host_id, callback);
}
