void FileReaderLoader::Cancel() {
  error_code_ = FileError::kAbortErr;
  Cleanup();
}
