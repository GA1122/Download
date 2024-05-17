  bool CanRequestURL(const GURL& url) {
    DCHECK(!url.SchemeIsBlob() && !url.SchemeIsFileSystem())
        << "inner_url extraction should be done already.";
    auto scheme_judgment = scheme_map_.find(url.scheme());
    if (scheme_judgment != scheme_map_.end())
      return true;

    if (CanRequestOrigin(url::Origin::Create(url)))
      return true;

    return CanCommitURL(url);
  }
