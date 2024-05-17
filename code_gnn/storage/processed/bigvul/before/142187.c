  base::FilePath GetTargetPathForTestEntry(
      const AddEntriesMessage::TestEntryInfo& entry) {
    const base::FilePath target_path =
        GetTargetBasePathForTestEntry(entry).Append(entry.target_path);
    if (entry.name_text != entry.target_path)
      return target_path.DirName().Append(entry.name_text);
    return target_path;
  }
