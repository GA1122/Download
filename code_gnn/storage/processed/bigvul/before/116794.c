std::string WebRTCAudioDeviceTest::GetTestDataPath(
    const FilePath::StringType& file_name) {
  FilePath path;
  EXPECT_TRUE(PathService::Get(content::DIR_TEST_DATA, &path));
  path = path.Append(file_name);
  EXPECT_TRUE(file_util::PathExists(path));
#ifdef OS_WIN
  return WideToUTF8(path.value());
#else
  return path.value();
#endif
}
