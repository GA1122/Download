std::unique_ptr<FileReaderLoader> FileReaderLoader::Create(
    ReadType read_type,
    FileReaderLoaderClient* client) {
  return std::make_unique<FileReaderLoader>(read_type, client);
}
