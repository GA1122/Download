void MultibufferDataSource::MediaIsPlaying() {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  media_has_played_ = true;
  cancel_on_defer_ = false;
  preload_ = AUTO;
  url_data_and_loading_state_.SetLoadingState(
      UrlData::UrlDataWithLoadingState::LoadingState::kHasPlayed);
  UpdateBufferSizes();
}
