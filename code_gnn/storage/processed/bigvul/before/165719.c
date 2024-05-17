void FileReaderLoader::SetEncoding(const String& encoding) {
  if (!encoding.IsEmpty())
    encoding_ = WTF::TextEncoding(encoding);
}
