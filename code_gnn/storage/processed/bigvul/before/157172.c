UrlData::UrlDataWithLoadingState::~UrlDataWithLoadingState() {
  SetLoadingState(LoadingState::kIdle);
}
