void GDataDirectoryService::RefreshFileInternal(
    scoped_ptr<GDataFile> fresh_file,
    GDataEntry* old_entry) {
  GDataDirectory* entry_parent = old_entry ? old_entry->parent() : NULL;
  if (entry_parent) {
    DCHECK_EQ(fresh_file->resource_id(), old_entry->resource_id());
    DCHECK(old_entry->AsGDataFile());

    entry_parent->RemoveEntry(old_entry);
    entry_parent->AddEntry(fresh_file.release());
  }
}
