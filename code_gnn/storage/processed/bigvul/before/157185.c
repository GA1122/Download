void WebMediaPlayerImpl::DataSourceInitialized(bool success) {
  DVLOG(1) << __func__;
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (observer_ && IsNewRemotePlaybackPipelineEnabled() && data_source_)
    observer_->OnDataSourceInitialized(data_source_->GetUrlAfterRedirects());

  if (!success) {
    SetNetworkState(WebMediaPlayer::kNetworkStateFormatError);
    media_metrics_provider_->OnError(PIPELINE_ERROR_NETWORK);

    UpdatePlayState();

    return;
  }

  if (IsStreaming() && preload_ > MultibufferDataSource::METADATA) {
    data_source_->SetPreload(MultibufferDataSource::METADATA);
  }

  StartPipeline();
}
