SyncPromoUI::Source GetSigninSource(const GURL& url, GURL* continue_url) {
  std::string value;
  net::GetValueForKeyInQuery(url, "service", &value);
  bool possibly_an_explicit_signin = value == "chromiumsync";

  GURL local_continue_url = SyncPromoUI::GetNextPageURLForSyncPromoURL(url);
  while (gaia::IsGaiaSignonRealm(local_continue_url.GetOrigin())) {
    GURL next_continue_url =
        SyncPromoUI::GetNextPageURLForSyncPromoURL(local_continue_url);
    if (!next_continue_url.is_valid())
      break;
    local_continue_url = next_continue_url;
  }

  if (continue_url && local_continue_url.is_valid()) {
    DCHECK(!continue_url->is_valid() || *continue_url == local_continue_url);
    *continue_url = local_continue_url;
  }

  return possibly_an_explicit_signin ?
      SyncPromoUI::GetSourceForSyncPromoURL(local_continue_url) :
      SyncPromoUI::SOURCE_UNKNOWN;
}
