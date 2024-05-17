bool PermissionsData::CanAccessPage(const Extension* extension,
                                    const GURL& document_url,
                                    const GURL& top_frame_url,
                                    int tab_id,
                                    int process_id,
                                    std::string* error) const {
  return CanRunOnPage(extension,
                      document_url,
                      top_frame_url,
                      tab_id,
                      process_id,
                      active_permissions()->explicit_hosts(),
                      error);
}
