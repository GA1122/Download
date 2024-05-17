void AudioInputRendererHost::DoHandleError(
    media::AudioInputController* controller,
    int error_code) {
  DLOG(WARNING) << "AudioInputRendererHost::DoHandleError(error_code="
                << error_code << ")";
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  DeleteEntryOnError(entry);
}
