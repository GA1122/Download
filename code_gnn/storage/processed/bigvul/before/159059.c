std::string GetMimeType(const base::FilePath& path) {
  std::string mime_type;
  net::GetMimeTypeFromFile(path, &mime_type);
  return mime_type;
}
