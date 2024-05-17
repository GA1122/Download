SpeechRecognitionManagerImpl::GetSession(int session_id) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  auto iter = sessions_.find(session_id);
  DCHECK(iter != sessions_.end());
  return iter->second.get();
}
