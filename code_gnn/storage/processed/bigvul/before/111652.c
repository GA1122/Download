void GDataDirectory::RemoveChildren() {
  for (GDataFileCollection::const_iterator iter = child_files_.begin();
       iter != child_files_.end(); ++iter) {
    if (root_)
      root_->RemoveEntryFromResourceMap(iter->second);
  }
  STLDeleteValues(&child_files_);
  child_files_.clear();

  for (GDataDirectoryCollection::iterator iter = child_directories_.begin();
       iter != child_directories_.end(); ++iter) {
    GDataDirectory* dir = iter->second;
    dir->RemoveChildren();
    if (root_)
      root_->RemoveEntryFromResourceMap(dir);
  }
  STLDeleteValues(&child_directories_);
  child_directories_.clear();
}
