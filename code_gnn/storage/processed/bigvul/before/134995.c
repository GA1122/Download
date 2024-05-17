AppCacheResponseWriter* AppCacheUpdateJob::CreateResponseWriter() {
  AppCacheResponseWriter* writer =
      storage_->CreateResponseWriter(manifest_url_,
                                                group_->group_id());
  stored_response_ids_.push_back(writer->response_id());
  return writer;
}
