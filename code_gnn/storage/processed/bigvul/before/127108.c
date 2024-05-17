void AudioRendererAlgorithm::Initialize(float initial_playback_rate,
                                        const AudioParameters& params,
                                        const base::Closure& callback) {
  CHECK(params.IsValid());
  DCHECK(!callback.is_null());

  channels_ = params.channels();
  samples_per_second_ = params.sample_rate();
  bytes_per_channel_ = params.bits_per_sample() / 8;
  bytes_per_frame_ = params.GetBytesPerFrame();
  request_read_cb_ = callback;
  SetPlaybackRate(initial_playback_rate);

  window_size_ =
      samples_per_second_ * bytes_per_channel_ * channels_ * kWindowDuration;
  AlignToFrameBoundary(&window_size_);

  bytes_in_crossfade_ =
      samples_per_second_ * bytes_per_channel_ * channels_ * kCrossfadeDuration;
  AlignToFrameBoundary(&bytes_in_crossfade_);

  crossfade_buffer_.reset(new uint8[bytes_in_crossfade_]);
}
