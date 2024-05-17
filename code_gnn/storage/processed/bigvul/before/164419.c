bool PermissionsData::IsRestrictedUrl(const GURL& document_url,
                                      std::string* error) const {
  if (CanExecuteScriptEverywhere(extension_id_, location_))
    return false;

  if (g_policy_delegate &&
      g_policy_delegate->IsRestrictedUrl(document_url, error)) {
    return true;
  }

  if (!URLPattern::IsValidSchemeForExtensions(document_url.scheme()) &&
      document_url.spec() != url::kAboutBlankURL) {
    if (error) {
      if (active_permissions().HasAPIPermission(APIPermission::kTab)) {
        *error = ErrorUtils::FormatErrorMessage(
            manifest_errors::kCannotAccessPageWithUrl, document_url.spec());
      } else {
        *error = manifest_errors::kCannotAccessPage;
      }
    }
    return true;
  }

  if (!ExtensionsClient::Get()->IsScriptableURL(document_url, error))
    return true;

  bool allow_on_chrome_urls = base::CommandLine::ForCurrentProcess()->HasSwitch(
                                  switches::kExtensionsOnChromeURLs);
  if (document_url.SchemeIs(content::kChromeUIScheme) &&
      !allow_on_chrome_urls) {
    if (error)
      *error = manifest_errors::kCannotAccessChromeUrl;
    return true;
  }

  if (document_url.SchemeIs(kExtensionScheme) &&
      document_url.host() != extension_id_ && !allow_on_chrome_urls) {
    if (error)
      *error = manifest_errors::kCannotAccessExtensionUrl;
    return true;
  }

  return false;
}
