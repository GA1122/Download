void AudioInputRendererHost::OnCreated(
    media::AudioInputController* controller) {
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      base::Bind(
          &AudioInputRendererHost::DoCompleteCreation,
          this,
          make_scoped_refptr(controller)));
}
