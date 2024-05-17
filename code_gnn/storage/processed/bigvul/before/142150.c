  virtual void CreateEntry(const AddEntriesMessage::TestEntryInfo& entry) {
    const base::FilePath path =
        base::FilePath::FromUTF8Unsafe(entry.target_path);
    const std::string target_name = path.BaseName().AsUTF8Unsafe();

    drive::FileError error = drive::FILE_ERROR_OK;
    std::unique_ptr<drive::ResourceEntry> parent_entry(
        new drive::ResourceEntry);

    if (!entry.team_drive_name.empty()) {
      integration_service_->file_system()->GetResourceEntry(
          drive::util::GetDriveTeamDrivesRootPath()
              .Append(entry.team_drive_name)
              .Append(path)
              .DirName(),
          google_apis::test_util::CreateCopyResultCallback(&error,
                                                           &parent_entry));
    } else {
      integration_service_->file_system()->GetResourceEntry(
          drive::util::GetDriveMyDriveRootPath().Append(path).DirName(),
          google_apis::test_util::CreateCopyResultCallback(&error,
                                                           &parent_entry));
    }
    content::RunAllTasksUntilIdle();
    ASSERT_EQ(drive::FILE_ERROR_OK, error);
    ASSERT_TRUE(parent_entry);

    google_apis::FileResourceCapabilities file_capabilities;
    file_capabilities.set_can_copy(entry.capabilities.can_copy);
    file_capabilities.set_can_delete(entry.capabilities.can_delete);
    file_capabilities.set_can_rename(entry.capabilities.can_rename);
    file_capabilities.set_can_add_children(entry.capabilities.can_add_children);
    file_capabilities.set_can_share(entry.capabilities.can_share);

    google_apis::TeamDriveCapabilities team_drive_capabilities;
    team_drive_capabilities.set_can_copy(entry.capabilities.can_copy);
    team_drive_capabilities.set_can_delete_team_drive(
        entry.capabilities.can_delete);
    team_drive_capabilities.set_can_rename_team_drive(
        entry.capabilities.can_rename);
    team_drive_capabilities.set_can_add_children(
        entry.capabilities.can_add_children);
    team_drive_capabilities.set_can_share(entry.capabilities.can_share);

    switch (entry.type) {
      case AddEntriesMessage::FILE:
        CreateFile(entry.source_file_name, parent_entry->resource_id(),
                   target_name, entry.mime_type,
                   entry.shared_option == AddEntriesMessage::SHARED ||
                       entry.shared_option == AddEntriesMessage::SHARED_WITH_ME,
                   entry.last_modified_time, file_capabilities);
        break;
      case AddEntriesMessage::DIRECTORY:
        CreateDirectory(
            parent_entry->resource_id(), target_name, entry.last_modified_time,
            entry.shared_option == AddEntriesMessage::SHARED ||
                entry.shared_option == AddEntriesMessage::SHARED_WITH_ME,
            file_capabilities);
        break;
      case AddEntriesMessage::TEAM_DRIVE:
        CreateTeamDrive(entry.team_drive_name, team_drive_capabilities);
        break;
      case AddEntriesMessage::COMPUTER:
        NOTREACHED() << "Can't create a computer in a drive test volume: "
                     << entry.computer_name;
    }

    CheckForUpdates();
    content::RunAllTasksUntilIdle();
  }
