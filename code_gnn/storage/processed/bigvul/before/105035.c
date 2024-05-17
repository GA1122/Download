void TestURLFetcher::SetResponseFilePath(const FilePath& path) {
  SetResponseDestinationForTesting(TEMP_FILE);
  fake_response_file_path_ = path;
}
