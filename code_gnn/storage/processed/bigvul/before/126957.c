void AudioInputRendererHost::OnData(media::AudioInputController* controller,
                                    const uint8* data,
                                    uint32 size) {
  NOTREACHED() << "Only low-latency mode is supported.";
}
