bool Extension::CanExecuteScriptOnPage(const GURL& document_url,
                                       const GURL& top_frame_url,
                                       int tab_id,
                                       const UserScript* script,
                                       std::string* error) const {
  base::AutoLock auto_lock(runtime_data_lock_);
  GURL store_url(extension_urls::GetWebstoreLaunchURL());
  if ((document_url.host() == store_url.host()) &&
      !CanExecuteScriptEverywhere() &&
      !CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kAllowScriptingGallery)) {
    if (error)
      *error = errors::kCannotScriptGallery;
    return false;
  }

  if (document_url.SchemeIs(chrome::kChromeUIScheme) &&
      !CanExecuteScriptEverywhere()) {
    return false;
  }

  if (top_frame_url.SchemeIs(extensions::kExtensionScheme) &&
      top_frame_url.GetOrigin() !=
          GetBaseURLFromExtensionId(id()).GetOrigin() &&
      !CanExecuteScriptEverywhere()) {
    return false;
  }

  if (tab_id >= 0) {
    scoped_refptr<const PermissionSet> tab_permissions =
        runtime_data_.GetTabSpecificPermissions(tab_id);
    if (tab_permissions.get() &&
        tab_permissions->explicit_hosts().MatchesSecurityOrigin(document_url)) {
      return true;
    }
  }

  if (script)
    return script->MatchesURL(document_url);

  if (runtime_data_.GetActivePermissions()->HasExplicitAccessToOrigin(
          document_url)) {
    return true;
  }

  if (error) {
    *error = ErrorUtils::FormatErrorMessage(errors::kCannotAccessPage,
                                                     document_url.spec());
  }

  return false;
}
