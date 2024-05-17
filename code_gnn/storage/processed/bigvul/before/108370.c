void AudioRendererHost::OnCreated(media::AudioOutputController* controller) {
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      NewRunnableMethod(
          this,
          &AudioRendererHost::DoCompleteCreation,
          make_scoped_refptr(controller)));
}
