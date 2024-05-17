VideoCaptureController* VideoCaptureManager::LookupControllerBySessionId(
    int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  SessionMap::const_iterator session_it = sessions_.find(session_id);
  if (session_it == sessions_.end())
    return nullptr;

  return LookupControllerByMediaTypeAndDeviceId(session_it->second.type,
                                                session_it->second.id);
}
