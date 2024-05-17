void AudioRendererHost::OnMoreData(media::AudioOutputController* controller,
                                   AudioBuffersState buffers_state) {
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      NewRunnableMethod(this,
                        &AudioRendererHost::DoRequestMoreData,
                        make_scoped_refptr(controller),
                        buffers_state));
}
