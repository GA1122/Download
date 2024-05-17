void AudioInputRendererHost::OnDeviceStarted(
    int session_id, const std::string& device_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  SessionEntryMap::iterator it = session_entries_.find(session_id);
  if (it == session_entries_.end()) {
    DLOG(WARNING) << "AudioInputRendererHost::OnDeviceStarted()"
        " session does not exist.";
    return;
  }

  Send(new AudioInputMsg_NotifyDeviceStarted(it->second, device_id));
}
