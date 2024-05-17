  bool UpdateModifiedTime(const AddEntriesMessage::TestEntryInfo& entry) {
    const auto path = GetTargetPathForTestEntry(entry);
    if (!base::TouchFile(path, entry.last_modified_time,
                         entry.last_modified_time)) {
      return false;
    }

    if (path.DirName() != GetTeamDriveGrandRoot() &&
        path.DirName() != GetComputerGrandRoot() &&
        path.DirName() != GetMyDrivePath() &&
        path.DirName() != GetSharedWithMePath()) {
      const auto it = entries_.find(path.DirName());
      if (it == entries_.end())
        return false;
      return UpdateModifiedTime(it->second);
    }

    return true;
  }
