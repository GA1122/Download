void AudioRendererHost::OnStreamClosed(AudioEntry* entry) {
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      NewRunnableMethod(this, &AudioRendererHost::DeleteEntry, entry));
}
