int WebRtcAudioRenderer::Render(media::AudioBus* audio_bus,
                                int audio_delay_milliseconds) {
  {
    base::AutoLock auto_lock(lock_);
    if (!source_)
      return 0;
    source_->RenderData(reinterpret_cast<uint8*>(buffer_.get()),
                        audio_bus->channels(), audio_bus->frames(),
                        audio_delay_milliseconds);

    if (state_ != PLAYING)
      return 0;
  }

  audio_bus->FromInterleaved(buffer_.get(), audio_bus->frames(),
                             params_.bits_per_sample() / 8);
  return audio_bus->frames();
}
