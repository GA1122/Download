bool GaiaCookieManagerService::ListAccounts(
    std::vector<gaia::ListedAccount>* accounts,
    std::vector<gaia::ListedAccount>* signed_out_accounts,
    const std::string& source) {
  if (!list_accounts_stale_) {
    if (accounts)
      accounts->assign(listed_accounts_.begin(), listed_accounts_.end());

    if (signed_out_accounts) {
      signed_out_accounts->assign(signed_out_accounts_.begin(),
                                  signed_out_accounts_.end());
    }

    return true;
  }

  TriggerListAccounts(source);
  return false;
}
