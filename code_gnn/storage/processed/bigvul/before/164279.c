  bool IsAllowed(const scoped_refptr<const Extension>& extension,
                 const GURL& url) {
    return IsAllowed(extension, url, PERMITTED_BOTH, tab_id());
  }
