AppCacheUpdateJob::CreateResponseWriter() {
  std::unique_ptr<AppCacheResponseWriter> writer =
      storage_->CreateResponseWriter(manifest_url_);
  stored_response_ids_.push_back(writer->response_id());
  return writer;
}
