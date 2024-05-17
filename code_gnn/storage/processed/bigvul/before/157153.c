void UrlData::UrlDataWithLoadingState::SetLoadingState(
    LoadingState loading_state) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!url_data_)
    return;
  url_data_->IncreaseLoadersInState(loading_state);
  url_data_->DecreaseLoadersInState(loading_state_);
  loading_state_ = loading_state;
}
