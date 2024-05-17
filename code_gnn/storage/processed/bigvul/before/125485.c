void GDataDirectory::AddChild(GDataEntry* entry) {
  DCHECK(entry);

  GDataFile* file = entry->AsGDataFile();
  if (file)
    child_files_.insert(std::make_pair(entry->base_name(), file));

  GDataDirectory* directory = entry->AsGDataDirectory();
  if (directory)
    child_directories_.insert(std::make_pair(entry->base_name(), directory));
}
