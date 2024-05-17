  void FindAndTestFilePath(const FilePath& file_path) {
    GDataEntry* entry = FindEntry(file_path);
    ASSERT_TRUE(entry) << "Entry can't be found " << file_path.value();
    EXPECT_EQ(entry->GetFilePath(), file_path);
  }
