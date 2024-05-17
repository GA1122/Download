  void InsertEntryOnMap(const AddEntriesMessage::TestEntryInfo& entry,
                        const base::FilePath& target_path) {
    const auto it = entries_.find(target_path);
    if (it == entries_.end())
      entries_.insert(std::make_pair(target_path, entry));
  }
