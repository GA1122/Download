void UrlFetcher::Core::SetUploadData(const std::string& upload_content_type,
                                     const std::string& upload_content) {
  upload_content_type_ = upload_content_type;
  upload_content_ = upload_content;
}
