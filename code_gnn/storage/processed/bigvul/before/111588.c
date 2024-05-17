  static void OnExpectToFindEntry(const FilePath& search_file_path,
                                  base::PlatformFileError error,
                                  const FilePath& directory_path,
                                  GDataEntry* entry) {
    ASSERT_TRUE(entry);
    if (entry->file_info().is_directory)
      ASSERT_EQ(search_file_path, directory_path);
    else
      ASSERT_EQ(search_file_path, directory_path.Append(entry->file_name()));
  }
