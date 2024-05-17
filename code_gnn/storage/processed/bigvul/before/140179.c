std::string GaiaCookieManagerService::GetSourceForRequest(
    const GaiaCookieManagerService::GaiaCookieRequest& request) {
  return request.source().empty() ? GetDefaultSourceForRequest() :
      request.source();
}
