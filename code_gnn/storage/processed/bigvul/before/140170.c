GaiaCookieManagerService::GaiaCookieRequest::CreateListAccountsRequest(
    const std::string& source) {
  return GaiaCookieManagerService::GaiaCookieRequest(
      GaiaCookieManagerService::GaiaCookieRequestType::LIST_ACCOUNTS,
      std::string(), source);
}
