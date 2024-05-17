GaiaCookieManagerService::GaiaCookieRequest::CreateAddAccountRequest(
    const std::string& account_id,
    const std::string& source) {
  return GaiaCookieManagerService::GaiaCookieRequest(
      GaiaCookieManagerService::GaiaCookieRequestType::ADD_ACCOUNT, account_id,
      source);
}
