GaiaCookieManagerService::GaiaCookieRequest::CreateLogOutRequest(
    const std::string& source) {
  return GaiaCookieManagerService::GaiaCookieRequest(
      GaiaCookieManagerService::GaiaCookieRequestType::LOG_OUT, std::string(),
      source);
}
