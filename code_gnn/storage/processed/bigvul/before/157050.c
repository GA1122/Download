void MultibufferDataSource::UpdateBufferSizes() {
  DVLOG(1) << __func__;
  if (!reader_)
    return;

  buffer_size_update_counter_ = kUpdateBufferSizeFrequency;

  int64_t bitrate = clamp<int64_t>(bitrate_, 0, kMaxBitrate);
  if (bitrate == 0)
    bitrate = kDefaultBitrate;

  double playback_rate = playback_rate_;

  playback_rate = std::max(playback_rate, 1.0);
  playback_rate = std::min(playback_rate, kMaxPlaybackRate);

  int64_t bytes_per_second = (bitrate / 8.0) * playback_rate;

  int64_t preload = clamp(kTargetSecondsBufferedAhead * bytes_per_second,
                          kMinBufferPreload, kMaxBufferPreload);

  int64_t extra_buffer = std::min(
      preload, url_data()->BytesReadFromCache() * kSlowPreloadPercentage / 100);

  preload += extra_buffer;

  int64_t preload_high = preload + kPreloadHighExtra;

  int64_t pin_backward = clamp(kTargetSecondsBufferedBehind * bytes_per_second,
                               kMinBufferPreload, kMaxBufferPreload);

  int64_t pin_forward = std::max(preload_high, kDefaultPinSize);

  int64_t buffer_size =
      std::min((kTargetSecondsBufferedAhead + kTargetSecondsBufferedBehind) *
                       bytes_per_second +
                   extra_buffer * 3,
               preload_high + pin_backward + extra_buffer);

  if (url_data()->FullyCached() ||
      (url_data()->length() != kPositionNotSpecified &&
       url_data()->length() < kDefaultPinSize)) {
    pin_forward = std::max(pin_forward, url_data()->length() * 2);
    pin_backward = std::max(pin_backward, url_data()->length() * 2);
    buffer_size = url_data()->length();
  }

  reader_->SetMaxBuffer(buffer_size);
  reader_->SetPinRange(pin_backward, pin_forward);

  if (preload_ == METADATA) {
    preload_high >>= kMetadataShift;
    preload >>= kMetadataShift;
  }
  reader_->SetPreload(preload_high, preload);
}
