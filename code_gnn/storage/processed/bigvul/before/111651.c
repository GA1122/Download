bool GDataDirectory::RemoveChild(GDataEntry* entry) {
  DCHECK(entry);

  const std::string file_name(entry->file_name());
  GDataEntry* found_entry = FindChild(file_name);
  if (!found_entry)
    return false;

  DCHECK_EQ(entry, found_entry);

  if (root_)
    root_->RemoveEntryFromResourceMap(entry);

  child_files_.erase(file_name);
  child_directories_.erase(file_name);

  return true;
}
