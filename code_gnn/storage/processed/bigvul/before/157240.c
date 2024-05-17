void WebMediaPlayerImpl::MaybeSetContainerName() {
  if (chunk_demuxer_ || using_media_player_renderer_)
    return;

  if (!demuxer_)
    return;

  if (highest_ready_state_ >= WebMediaPlayer::kReadyStateHaveMetadata)
    return;

#if BUILDFLAG(ENABLE_FFMPEG)
  media_metrics_provider_->SetContainerName(
      static_cast<FFmpegDemuxer*>(demuxer_.get())->container());
#endif
}
