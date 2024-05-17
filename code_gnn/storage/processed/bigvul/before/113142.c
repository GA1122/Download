content::MockDownloadItem* MockDownloadItemFactory::PopItem() {
  if (items_.empty())
    return NULL;

  std::map<int32, content::MockDownloadItem*>::iterator first_item
      = items_.begin();
  content::MockDownloadItem* result = first_item->second;
  items_.erase(first_item);
  return result;
}
