void AudioRendererHost::OnError(media::AudioOutputController* controller,
                                int error_code) {
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      NewRunnableMethod(this,
                        &AudioRendererHost::DoHandleError,
                        make_scoped_refptr(controller),
                        error_code));
}
