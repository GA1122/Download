void GDataDirectory::AddEntry(GDataEntry* entry) {
  entry->SetFileNameFromTitle();

  int max_modifier = 1;
  FilePath full_file_name(entry->file_name());
  const std::string extension = full_file_name.Extension();
  const std::string file_name = full_file_name.RemoveExtension().value();
  while (FindChild(full_file_name.value())) {
    if (!extension.empty()) {
      full_file_name = FilePath(base::StringPrintf("%s (%d)%s",
                                                   file_name.c_str(),
                                                   ++max_modifier,
                                                   extension.c_str()));
    } else {
      full_file_name = FilePath(base::StringPrintf("%s (%d)",
                                                   file_name.c_str(),
                                                   ++max_modifier));
    }
  }
  entry->set_file_name(full_file_name.value());

  DVLOG(1) << "AddEntry: dir = " << GetFilePath().value()
           << ", file = " + entry->file_name()
           << ", parent resource = " << entry->parent_resource_id()
           << ", resource = " + entry->resource_id();


  if (root_)
    root_->AddEntryToResourceMap(entry);

  AddChild(entry);
  entry->SetParent(this);
}
