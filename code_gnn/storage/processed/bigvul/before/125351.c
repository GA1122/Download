void CreateDocumentJsonFileOnBlockingPool(
    const FilePath& document_dir,
    const GURL& edit_url,
    const std::string& resource_id,
    GDataFileError* error,
    FilePath* temp_file_path,
    std::string* mime_type,
    GDataFileType* file_type) {
  DCHECK(error);
  DCHECK(temp_file_path);
  DCHECK(mime_type);
  DCHECK(file_type);

  *error = GDATA_FILE_ERROR_FAILED;

  if (file_util::CreateTemporaryFileInDir(document_dir, temp_file_path)) {
    std::string document_content = base::StringPrintf(
        "{\"url\": \"%s\", \"resource_id\": \"%s\"}",
        edit_url.spec().c_str(), resource_id.c_str());
    int document_size = static_cast<int>(document_content.size());
    if (file_util::WriteFile(*temp_file_path, document_content.data(),
                             document_size) == document_size) {
      *error = GDATA_FILE_OK;
    }
  }

  *mime_type = kMimeTypeJson;
  *file_type = HOSTED_DOCUMENT;
  if (*error != GDATA_FILE_OK)
      temp_file_path->clear();
}
