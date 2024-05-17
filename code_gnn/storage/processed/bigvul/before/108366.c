AudioRendererHost::AudioEntry* AudioRendererHost::LookupById(
    int route_id, int stream_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntryMap::iterator i = audio_entries_.find(
      AudioEntryId(route_id, stream_id));
  if (i != audio_entries_.end())
    return i->second;
  return NULL;
}
