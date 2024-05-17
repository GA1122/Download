scoped_refptr<UrlData> UrlIndex::GetByUrl(const GURL& gurl,
                                          UrlData::CORSMode cors_mode) {
  auto i = indexed_data_.find(std::make_pair(gurl, cors_mode));
  if (i != indexed_data_.end() && i->second->Valid()) {
    return i->second;
  }

  return NewUrlData(gurl, cors_mode);
}
