void UrlIndex::RemoveUrlData(const scoped_refptr<UrlData>& url_data) {
  DCHECK(url_data->multibuffer()->map().empty());

  auto i = indexed_data_.find(url_data->key());
  if (i != indexed_data_.end() && i->second == url_data)
    indexed_data_.erase(i);

  RemoveLoading(url_data.get());
}
