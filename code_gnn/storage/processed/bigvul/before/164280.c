  bool IsAllowed(const scoped_refptr<const Extension>& extension,
                 const GURL& url,
                 PermittedFeature feature) {
    return IsAllowed(extension, url, feature, tab_id());
  }
