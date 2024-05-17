void UrlFetcher::SetUploadData(const std::string& upload_content_type,
                               const std::string& upload_content) {
  core_->SetUploadData(upload_content_type, upload_content);
}
