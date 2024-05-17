void BackgroundLoaderOfflinerTest::OnCompletion(
    const SavePageRequest& request,
    Offliner::RequestStatus status) {
  DCHECK(!completion_callback_called_);   
  completion_callback_called_ = true;
  request_status_ = status;
}
