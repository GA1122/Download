void GDataDirectory::RemoveChild(GDataEntry* entry) {
  DCHECK(entry);

  const std::string& base_name(entry->base_name());
  DCHECK_EQ(entry, FindChild(base_name));
  if (directory_service_)
    directory_service_->RemoveEntryFromResourceMap(entry);

  child_files_.erase(base_name);
  child_directories_.erase(base_name);
}
