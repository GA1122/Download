void BackgroundLoaderOffliner::WebContentsDestroyed() {
  if (pending_request_) {
    SavePageRequest request(*pending_request_.get());
    std::move(completion_callback_)
        .Run(request, Offliner::RequestStatus::LOADING_FAILED);
    ResetState();
  }
}
