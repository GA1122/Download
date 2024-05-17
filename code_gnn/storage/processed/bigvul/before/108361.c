void AudioRendererHost::DoHandleError(media::AudioOutputController* controller,
                                      int error_code) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  DeleteEntryOnError(entry);
}
