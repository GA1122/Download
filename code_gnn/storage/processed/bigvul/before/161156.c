void MediaStreamManager::SendMessageToNativeLog(const std::string& message) {
  if (!BrowserThread::CurrentlyOn(BrowserThread::IO)) {
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::BindOnce(&MediaStreamManager::SendMessageToNativeLog, message));
    return;
  }

  MediaStreamManager* msm = g_media_stream_manager_tls_ptr.Pointer()->Get();
  if (!msm) {
    return;
  }

  msm->AddLogMessageOnIOThread(message);
}
