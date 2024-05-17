int AudioInputRendererHost::LookupSessionById(int stream_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  for (SessionEntryMap::iterator it = session_entries_.begin();
       it != session_entries_.end(); ++it) {
    if (stream_id == it->second) {
      return it->first;
    }
  }
  return 0;
}
