bool MockNetworkTransaction::IsReadyToRestartForAuth() {
  if (!request_)
    return false;

  if (!request_->extra_headers.HasHeader("X-Require-Mock-Auth"))
    return false;

  std::string status_line = response_.headers->GetStatusLine();
  return status_line.find(" 401 ") != std::string::npos ||
      status_line.find(" 407 ") != std::string::npos;
}
