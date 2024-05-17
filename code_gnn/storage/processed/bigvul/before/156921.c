void DocumentLoader::StopLoading() {
  fetcher_->StopFetching();
  if (frame_ && !SentDidFinishLoad())
    LoadFailed(ResourceError::CancelledError(Url()));
}
