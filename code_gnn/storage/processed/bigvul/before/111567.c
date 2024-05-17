void AsyncInitializationCallback(
    int* counter,
    int expected_counter,
    const FilePath& expected_file_path,
    MessageLoop* message_loop,
    base::PlatformFileError error,
    scoped_ptr<GDataDirectoryProto> directory_proto) {
  ASSERT_EQ(base::PLATFORM_FILE_OK, error);
  ASSERT_TRUE(directory_proto.get());
  EXPECT_EQ(expected_file_path.value(),
            directory_proto->gdata_entry().file_name());

  (*counter)++;
  if (*counter >= expected_counter)
    message_loop->Quit();
}
