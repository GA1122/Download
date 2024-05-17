bool ExtensionCanAttachToURL(const Extension& extension,
                             const GURL& url,
                             Profile* profile,
                             std::string* error) {
  if (url == content::kUnreachableWebDataURL)
    return true;

  if (extension.permissions_data()->IsRestrictedUrl(url, error))
    return false;

  if (url.SchemeIsFile() && !util::AllowFileAccess(extension.id(), profile)) {
    *error = debugger_api_constants::kRestrictedError;
    return false;
  }

  return true;
}
