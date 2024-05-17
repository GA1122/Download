 void SecureProxyChecker::OnURLLoadComplete(
     std::unique_ptr<std::string> response_body) {
  std::string response;
  if (response_body)
    response = std::move(*response_body);

  int response_code = -1;
  if (url_loader_->ResponseInfo() && url_loader_->ResponseInfo()->headers)
    response_code = url_loader_->ResponseInfo()->headers->response_code();

  OnURLLoadCompleteOrRedirect(response, url_loader_->NetError(), response_code);
}
