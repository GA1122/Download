void FileReaderLoader::Cancel() {
  error_code_ = FileErrorCode::kAbortErr;
  Cleanup();
}
