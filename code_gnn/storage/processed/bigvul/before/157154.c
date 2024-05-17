void UrlData::UrlDataWithLoadingState::SetUrlData(
    scoped_refptr<UrlData> url_data) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (url_data)
    url_data->IncreaseLoadersInState(loading_state_);
  if (url_data_)
    url_data_->DecreaseLoadersInState(loading_state_);
  url_data_ = std::move(url_data);
}
