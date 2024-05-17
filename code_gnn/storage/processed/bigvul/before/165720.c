void FileReaderLoader::SetStringResult(const String& result) {
  AdjustReportedMemoryUsageToV8(
      -1 * static_cast<int64_t>(string_result_.CharactersSizeInBytes()));
  is_raw_data_converted_ = true;
  string_result_ = result;
  AdjustReportedMemoryUsageToV8(string_result_.CharactersSizeInBytes());
}
