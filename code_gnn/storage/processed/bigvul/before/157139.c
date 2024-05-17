bool IsNewDataForSameResource(const scoped_refptr<UrlData>& new_entry,
                              const scoped_refptr<UrlData>& old_entry) {
  if (IsStrongEtag(new_entry->etag()) && IsStrongEtag(old_entry->etag())) {
    if (new_entry->etag() != old_entry->etag())
      return true;
  }
  if (!new_entry->last_modified().is_null()) {
    if (new_entry->last_modified() != old_entry->last_modified())
      return true;
  }
  return false;
}
