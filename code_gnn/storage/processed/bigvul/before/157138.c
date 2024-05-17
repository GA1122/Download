void UrlData::IncreaseLoadersInState(
    UrlDataWithLoadingState::LoadingState state) {
  switch (state) {
    case UrlDataWithLoadingState::LoadingState::kIdle:
      break;
    case UrlDataWithLoadingState::LoadingState::kPreload:
      preloading_++;
      break;
    case UrlDataWithLoadingState::LoadingState::kHasPlayed:
      playing_++;
      if (playing_ == 1)
        url_index_->RemoveLoading(this);
      break;
  }
}
