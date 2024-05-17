  void CreateDirectory(
      const std::string& parent_id,
      const std::string& target_name,
      const base::Time& modification_time,
      bool shared_with_me,
      const google_apis::FileResourceCapabilities& capabilities) {
    google_apis::DriveApiErrorCode error = google_apis::DRIVE_OTHER_ERROR;

    std::unique_ptr<google_apis::FileResource> entry;
    fake_drive_service_->AddNewDirectory(
        parent_id, target_name, drive::AddNewDirectoryOptions(),
        google_apis::test_util::CreateCopyResultCallback(&error, &entry));
    base::RunLoop().RunUntilIdle();
    ASSERT_EQ(google_apis::HTTP_CREATED, error);
    ASSERT_TRUE(entry);

    fake_drive_service_->SetLastModifiedTime(
        entry->file_id(), modification_time,
        google_apis::test_util::CreateCopyResultCallback(&error, &entry));
    base::RunLoop().RunUntilIdle();
    ASSERT_TRUE(error == google_apis::HTTP_SUCCESS);
    ASSERT_TRUE(entry);

    fake_drive_service_->SetFileCapabilities(
        entry->file_id(), capabilities,
        google_apis::test_util::CreateCopyResultCallback(&error, &entry));
    base::RunLoop().RunUntilIdle();
    ASSERT_TRUE(error == google_apis::HTTP_SUCCESS);
    ASSERT_TRUE(entry);

    if (shared_with_me) {
      ASSERT_EQ(google_apis::HTTP_SUCCESS,
                fake_drive_service_->SetFileAsSharedWithMe(entry->file_id()));
    }
  }
