void ContentSearchCallback(MessageLoop* message_loop,
                           base::PlatformFileError error,
                           scoped_ptr<gdata::GDataDirectoryProto> dir_proto) {
  ASSERT_EQ(base::PLATFORM_FILE_OK, error);
  ASSERT_TRUE(dir_proto.get());

  ASSERT_EQ(1, dir_proto->child_files_size());
  EXPECT_EQ("file:2_file_resouce_id.SubDirectory File 1.txt",
            dir_proto->child_files(0).gdata_entry().file_name());

  ASSERT_EQ(1, dir_proto->child_directories_size());
  EXPECT_EQ("folder:1_folder_resource_id.Directory 1",
            dir_proto->child_directories(0).gdata_entry().file_name());

  message_loop->Quit();
}
