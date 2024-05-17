media::AudioSystem* MediaStreamManager::audio_system() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  return audio_system_;
}
