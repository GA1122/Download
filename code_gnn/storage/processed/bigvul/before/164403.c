bool PermissionsData::CanRunContentScriptOnPage(const GURL& document_url,
                                                int tab_id,
                                                std::string* error) const {
  PageAccess result = GetContentScriptAccess(document_url, tab_id, error);

  return result == PageAccess::kAllowed || result == PageAccess::kWithheld;
}
