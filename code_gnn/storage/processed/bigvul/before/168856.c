bool PermissionsData::CanAccessPage(const Extension* extension,
                                    const GURL& document_url,
                                    int tab_id,
                                    std::string* error) const {
  AccessType result = GetPageAccess(extension, document_url, tab_id, error);

  return result == ACCESS_ALLOWED || result == ACCESS_WITHHELD;
}
