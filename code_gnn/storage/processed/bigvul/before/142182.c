  base::FilePath GetRelativeDrivePathForTestEntry(
      const AddEntriesMessage::TestEntryInfo& entry) {
    const base::FilePath target_path = GetTargetPathForTestEntry(entry);
    base::FilePath drive_path("/");
    CHECK(mount_path().AppendRelativePath(target_path, &drive_path));
    return drive_path;
  }
