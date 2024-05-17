scoped_refptr<UrlData> UrlIndex::TryInsert(
    const scoped_refptr<UrlData>& url_data) {
  auto iter = indexed_data_.find(url_data->key());
  if (iter == indexed_data_.end()) {
    if (url_data->Valid()) {
      indexed_data_.insert(iter, std::make_pair(url_data->key(), url_data));
    }
    return url_data;
  }


  if (iter->second == url_data)
    return url_data;

  if (IsNewDataForSameResource(url_data, iter->second)) {
    if (url_data->Valid()) {
      iter->second = url_data;
    }
    return url_data;
  }

  if (url_data->Valid()) {
    if ((!iter->second->Valid() ||
         url_data->CachedSize() > iter->second->CachedSize())) {
      iter->second = url_data;
    } else {
      iter->second->MergeFrom(url_data);
    }
  }
  return iter->second;
}
