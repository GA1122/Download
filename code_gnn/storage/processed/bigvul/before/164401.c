bool PermissionsData::CanAccessPage(const GURL& document_url,
                                    int tab_id,
                                    std::string* error) const {
  PageAccess result = GetPageAccess(document_url, tab_id, error);

  return result == PageAccess::kAllowed || result == PageAccess::kWithheld;
}
