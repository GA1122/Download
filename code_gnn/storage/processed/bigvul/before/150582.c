void DataReductionProxyConfigServiceClient::OnURLLoadComplete(
    std::unique_ptr<std::string> response_body) {
  DCHECK(thread_checker_.CalledOnValidThread());
  fetch_in_progress_ = false;

  int response_code = -1;
  if (url_loader_->ResponseInfo() && url_loader_->ResponseInfo()->headers) {
    response_code = url_loader_->ResponseInfo()->headers->response_code();
  }
  HandleResponse(response_body ? *response_body : "", url_loader_->NetError(),
                 response_code);
}
