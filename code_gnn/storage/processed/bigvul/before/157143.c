scoped_refptr<UrlData> UrlIndex::NewUrlData(const GURL& url,
                                            UrlData::CORSMode cors_mode) {
  return new UrlData(url, cors_mode, this);
}
