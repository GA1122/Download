  bool IsBlocked(const scoped_refptr<const Extension>& extension,
                 const GURL& url) {
    return IsBlocked(extension, url, tab_id());
  }
