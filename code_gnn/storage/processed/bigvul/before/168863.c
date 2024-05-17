bool PermissionsData::IsRestrictedUrl(const GURL& document_url,
                                      const Extension* extension,
                                      std::string* error) {
  if (extension && CanExecuteScriptEverywhere(extension))
    return false;

  if (!URLPattern::IsValidSchemeForExtensions(document_url.scheme()) &&
      document_url.spec() != url::kAboutBlankURL) {
    if (error) {
      if (extension->permissions_data()->active_permissions().HasAPIPermission(
              APIPermission::kTab)) {
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

  if (extension && document_url.SchemeIs(kExtensionScheme) &&
      document_url.host() != extension->id() && !allow_on_chrome_urls) {
    if (error)
      *error = manifest_errors::kCannotAccessExtensionUrl;
    return true;
  }

  return false;
}
