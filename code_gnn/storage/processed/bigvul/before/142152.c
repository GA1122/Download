  void CreateEntryImpl(const AddEntriesMessage::TestEntryInfo& entry,
                       const base::FilePath& target_path) {
    entries_.insert(std::make_pair(target_path, entry));
    switch (entry.type) {
      case AddEntriesMessage::FILE: {
        const base::FilePath source_path =
            TestVolume::GetTestDataFilePath(entry.source_file_name);
        ASSERT_TRUE(base::CopyFile(source_path, target_path))
            << "Copy from " << source_path.value() << " to "
            << target_path.value() << " failed.";
        break;
      }
      case AddEntriesMessage::DIRECTORY:
        ASSERT_TRUE(base::CreateDirectory(target_path))
            << "Failed to create a directory: " << target_path.value();
        break;
      case AddEntriesMessage::TEAM_DRIVE:
        NOTREACHED() << "Can't create a team drive in a local volume: "
                     << target_path.value();
        break;
      case AddEntriesMessage::COMPUTER:
        NOTREACHED() << "Can't create a computer in a local volume: "
                     << target_path.value();
        break;
      default:
        NOTREACHED() << "Unsupported entry type for: " << target_path.value();
    }

    ASSERT_TRUE(UpdateModifiedTime(entry, target_path));
  }
