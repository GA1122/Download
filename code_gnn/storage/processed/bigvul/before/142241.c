  bool UpdateModifiedTime(const AddEntriesMessage::TestEntryInfo& entry,
                          const base::FilePath& path) {
    if (!base::TouchFile(path, entry.last_modified_time,
                         entry.last_modified_time)) {
      return false;
    }

    if (path.DirName() != root_path()) {
      const auto& it = entries_.find(path.DirName());
      if (it == entries_.end())
        return false;
      return UpdateModifiedTime(it->second, path.DirName());
    }

    return true;
  }
