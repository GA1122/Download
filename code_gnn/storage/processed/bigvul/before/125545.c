void VerifyDirectoryService(GDataDirectoryService* directory_service) {
  ASSERT_TRUE(directory_service->root());

  GDataDirectory* dir1 = FindDirectory(directory_service, "drive/dir1");
  ASSERT_TRUE(dir1);
  GDataDirectory* dir2 = FindDirectory(directory_service, "drive/dir2");
  ASSERT_TRUE(dir2);
  GDataDirectory* dir3 = FindDirectory(directory_service, "drive/dir1/dir3");
  ASSERT_TRUE(dir3);

  GDataFile* file4 = FindFile(directory_service, "drive/dir1/file4");
  ASSERT_TRUE(file4);
  EXPECT_EQ(file4->parent(), dir1);

  GDataFile* file5 = FindFile(directory_service, "drive/dir1/file5");
  ASSERT_TRUE(file5);
  EXPECT_EQ(file5->parent(), dir1);

  GDataFile* file6 = FindFile(directory_service, "drive/dir2/file6");
  ASSERT_TRUE(file6);
  EXPECT_EQ(file6->parent(), dir2);

  GDataFile* file7 = FindFile(directory_service, "drive/dir2/file7");
  ASSERT_TRUE(file7);
  EXPECT_EQ(file7->parent(), dir2);

  GDataFile* file8 = FindFile(directory_service, "drive/dir2/file8");
  ASSERT_TRUE(file8);
  EXPECT_EQ(file8->parent(), dir2);

  GDataFile* file9 = FindFile(directory_service, "drive/dir1/dir3/file9");
  ASSERT_TRUE(file9);
  EXPECT_EQ(file9->parent(), dir3);

  GDataFile* file10 = FindFile(directory_service, "drive/dir1/dir3/file10");
  ASSERT_TRUE(file10);
  EXPECT_EQ(file10->parent(), dir3);

  EXPECT_EQ(dir1, directory_service->GetEntryByResourceId(
      "dir_resource_id:dir1"));
  EXPECT_EQ(dir2, directory_service->GetEntryByResourceId(
      "dir_resource_id:dir2"));
  EXPECT_EQ(dir3, directory_service->GetEntryByResourceId(
      "dir_resource_id:dir3"));
  EXPECT_EQ(file4, directory_service->GetEntryByResourceId(
      "file_resource_id:file4"));
  EXPECT_EQ(file5, directory_service->GetEntryByResourceId(
      "file_resource_id:file5"));
  EXPECT_EQ(file6, directory_service->GetEntryByResourceId(
      "file_resource_id:file6"));
  EXPECT_EQ(file7, directory_service->GetEntryByResourceId(
      "file_resource_id:file7"));
  EXPECT_EQ(file8, directory_service->GetEntryByResourceId(
      "file_resource_id:file8"));
  EXPECT_EQ(file9, directory_service->GetEntryByResourceId(
      "file_resource_id:file9"));
  EXPECT_EQ(file10, directory_service->GetEntryByResourceId(
      "file_resource_id:file10"));
}
