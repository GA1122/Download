void Automation::SetCookieDeprecated(int tab_id,
                                     const GURL& gurl,
                                     const std::string& cookie,
                                     bool* success) {
  int windex = 0, tab_index = 0;
  scoped_ptr<Error> error(GetIndicesForTab(tab_id, &windex, &tab_index));
  if (error.get()) {
    *success = false;
    return;
  }

  *success = SendSetCookieJSONRequestDeprecated(
      automation(),
      windex,
      gurl.possibly_invalid_spec(),
      cookie);
}
