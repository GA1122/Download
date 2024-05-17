void GDataWapiFeedProcessor::ApplyFeedFromFileUrlMap(
    bool is_delta_feed,
    int64 feed_changestamp,
    FileResourceIdMap* file_map,
  std::set<FilePath>* changed_dirs) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(changed_dirs);

  if (!is_delta_feed) {   
    directory_service_->root()->RemoveChildren();
    changed_dirs->insert(directory_service_->root()->GetFilePath());
  }
  directory_service_->set_largest_changestamp(feed_changestamp);

  scoped_ptr<GDataDirectoryService> orphaned_dir_service(
      new GDataDirectoryService);
  for (FileResourceIdMap::iterator it = file_map->begin();
       it != file_map->end();) {
    scoped_ptr<GDataEntry> entry(it->second);
    DCHECK_EQ(it->first, entry->resource_id());
    file_map->erase(it++);

    GDataEntry* old_entry =
        directory_service_->GetEntryByResourceId(entry->resource_id());
    GDataDirectory* dest_dir = NULL;
    if (entry->is_deleted()) {   
      DVLOG(1) << "Removing file " << entry->base_name();
      if (!old_entry)
        continue;

      dest_dir = old_entry->parent();
      if (!dest_dir) {
        NOTREACHED();
        continue;
      }
      RemoveEntryFromDirectoryAndCollectChangedDirectories(
          dest_dir, old_entry, changed_dirs);
    } else if (old_entry) {   
      DVLOG(1) << "Changed file " << entry->base_name();
      dest_dir = old_entry->parent();
      if (!dest_dir) {
        NOTREACHED();
        continue;
      }
      if (old_entry->AsGDataDirectory() && entry->AsGDataDirectory()) {
        entry->AsGDataDirectory()->TakeOverEntries(
            old_entry->AsGDataDirectory());
      }
      RemoveEntryFromDirectoryAndCollectChangedDirectories(
          dest_dir, old_entry, changed_dirs);
      if (dest_dir->resource_id() != entry->parent_resource_id()) {
        changed_dirs->insert(dest_dir->GetFilePath());
        dest_dir = FindDirectoryForNewEntry(entry.get(),
                                            *file_map,
                                            orphaned_dir_service.get());
      }
      DCHECK(dest_dir);
      AddEntryToDirectoryAndCollectChangedDirectories(
          entry.release(),
          dest_dir,
          orphaned_dir_service.get(),
          changed_dirs);
    } else {   
      dest_dir = FindDirectoryForNewEntry(entry.get(),
                                          *file_map,
                                          orphaned_dir_service.get());
      DCHECK(dest_dir);
      AddEntryToDirectoryAndCollectChangedDirectories(
          entry.release(),
          dest_dir,
          orphaned_dir_service.get(),
          changed_dirs);
    }

    if (dest_dir && (dest_dir->parent() ||
        dest_dir == directory_service_->root()) &&
        dest_dir != orphaned_dir_service->root() && is_delta_feed) {
      changed_dirs->insert(dest_dir->GetFilePath());
    }
  }
  DCHECK(file_map->empty());
}
