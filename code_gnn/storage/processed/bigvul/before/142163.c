  void EnsureDownloadsFolderExists() {
    if (!base::FeatureList::IsEnabled(chromeos::features::kMyFilesVolume))
      return;

    auto downloads_folder = root_path().Append("Downloads");
    auto downloads_entry = AddEntriesMessage::TestEntryInfo(
        AddEntriesMessage::DIRECTORY, "", "Downloads");
    if (!base::PathExists(downloads_folder))
      CreateEntryImpl(downloads_entry, downloads_folder);

    InsertEntryOnMap(downloads_entry, downloads_folder);
  }
