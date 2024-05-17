void GaiaCookieManagerService::OnUbertokenFailure(
    const GoogleServiceAuthError& error) {
  VLOG(1) << "Failed to retrieve ubertoken"
          << " account=" << requests_.front().account_id()
          << " error=" << error.ToString();
  const std::string account_id = requests_.front().account_id();
  HandleNextRequest();
  SignalComplete(account_id, error);
}
