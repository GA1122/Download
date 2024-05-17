void GDataDirectoryService::ClearRoot() {
  root_->RemoveChildren();
  RemoveEntryFromResourceMap(root_.get());
  DCHECK(resource_map_.empty());
  resource_map_.clear();
  root_.reset();
}
