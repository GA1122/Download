void GaiaCookieManagerService::SignalComplete(
    const std::string& account_id,
    const GoogleServiceAuthError& error) {
  for (auto& observer : observer_list_)
    observer.OnAddAccountToCookieCompleted(account_id, error);
}
