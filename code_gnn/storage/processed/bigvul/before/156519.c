  bool CanCommitURL(const GURL& url) {
    DCHECK(!url.SchemeIsBlob() && !url.SchemeIsFileSystem())
        << "inner_url extraction should be done already.";
    auto scheme_judgment = scheme_map_.find(url.scheme());
    if (scheme_judgment != scheme_map_.end() &&
        scheme_judgment->second == CommitRequestPolicy::kCommitAndRequest) {
      return true;
    }

    if (CanCommitOrigin(url::Origin::Create(url)))
      return true;

    if (url.SchemeIs(url::kFileScheme)) {
      base::FilePath path;
      if (net::FileURLToFilePath(url, &path))
        return base::ContainsKey(request_file_set_, path);
    }

    return false;   
  }
