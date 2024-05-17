bool GDataRootDirectory::ModifyFindEntryParamsForSearchPath(
    const FilePath& file_path,
    std::vector<FilePath::StringType>* components,
    GDataDirectory** current_dir,
    FilePath* directory_path) {
  DCHECK(current_dir);
  DCHECK(components);
  DCHECK_GT(components->size(), 3u);
  DCHECK(components->at(0) == "drive" && components->at(1) == ".search");

  FilePath::StringType resource_id;
  FilePath::StringType file_name;
  if (!util::ParseSearchFileName((*components)[3], &resource_id, &file_name))
    return false;

  GDataEntry* file_entry = GetEntryByResourceId(resource_id);
  if (!file_entry)
    return false;

  DCHECK(file_entry->parent());
  *current_dir = file_entry->parent();

  if ((*current_dir)->parent()) {
    *directory_path = (*current_dir)->parent()->GetFilePath();
  } else {
    *directory_path = FilePath();
  }

  components->erase(components->begin(), components->begin() + 2);
  (*components)[0] = (*current_dir)->file_name();
  (*components)[1] = file_name;
  return true;
}
