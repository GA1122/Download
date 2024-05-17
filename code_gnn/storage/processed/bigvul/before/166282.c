void VideoCaptureManager::OnScreenLocked() {
#if !defined(OS_ANDROID)
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  EmitLogMessage("VideoCaptureManager::OnScreenLocked", 1);

  std::vector<media::VideoCaptureSessionId> desktopcapture_session_ids;
  for (auto it : sessions_) {
    if (IsDesktopCaptureMediaType(it.second.type))
      desktopcapture_session_ids.push_back(it.first);
  }

  for (auto session_id : desktopcapture_session_ids) {
    Close(session_id);
  }
#endif   
}
