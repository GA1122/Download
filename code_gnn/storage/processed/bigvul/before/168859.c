PermissionsData::AccessType PermissionsData::CanRunOnPage(
    const Extension* extension,
    const GURL& document_url,
    int tab_id,
    const URLPatternSet& permitted_url_patterns,
    const URLPatternSet& withheld_url_patterns,
    const URLPatternSet* tab_url_patterns,
    std::string* error) const {
  runtime_lock_.AssertAcquired();
  if (g_policy_delegate && !g_policy_delegate->CanExecuteScriptOnPage(
                               extension, document_url, tab_id, error))
    return ACCESS_DENIED;

  if (extension->location() != Manifest::COMPONENT &&
      extension->permissions_data()->IsRuntimeBlockedHostUnsafe(document_url)) {
    if (error)
      *error = extension_misc::kPolicyBlockedScripting;
    return ACCESS_DENIED;
  }

  if (IsRestrictedUrl(document_url, extension, error))
    return ACCESS_DENIED;

  if (tab_url_patterns && tab_url_patterns->MatchesURL(document_url))
    return ACCESS_ALLOWED;

  if (permitted_url_patterns.MatchesURL(document_url))
    return ACCESS_ALLOWED;

  if (withheld_url_patterns.MatchesURL(document_url))
    return ACCESS_WITHHELD;

  if (error) {
    if (extension->permissions_data()->active_permissions().HasAPIPermission(
            APIPermission::kTab)) {
      *error = ErrorUtils::FormatErrorMessage(
          manifest_errors::kCannotAccessPageWithUrl, document_url.spec());
    } else {
      *error = manifest_errors::kCannotAccessPage;
    }
  }

  return ACCESS_DENIED;
}
