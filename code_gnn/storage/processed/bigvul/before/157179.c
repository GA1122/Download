bool WebMediaPlayerImpl::CanPlayThrough() {
  if (!base::FeatureList::IsEnabled(kSpecCompliantCanPlayThrough))
    return true;
  if (chunk_demuxer_)
    return true;
  if (data_source_ && data_source_->assume_fully_buffered())
    return true;
  if (network_state_ == WebMediaPlayer::kNetworkStateIdle)
    return true;
  return buffered_data_source_host_.CanPlayThrough(
      base::TimeDelta::FromSecondsD(CurrentTime()),
      base::TimeDelta::FromSecondsD(Duration()),
      playback_rate_ == 0.0 ? 1.0 : playback_rate_);
}
