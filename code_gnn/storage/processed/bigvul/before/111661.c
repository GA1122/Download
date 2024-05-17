bool GDataDirectory::TakeOverEntries(GDataDirectory* dir) {
  for (GDataFileCollection::iterator iter = dir->child_files_.begin();
       iter != dir->child_files_.end(); ++iter) {
    AddEntry(iter->second);
  }
  dir->child_files_.clear();

  for (GDataDirectoryCollection::iterator iter =
      dir->child_directories_.begin();
       iter != dir->child_directories_.end(); ++iter) {
    AddEntry(iter->second);
  }
  dir->child_directories_.clear();
  return true;
}
