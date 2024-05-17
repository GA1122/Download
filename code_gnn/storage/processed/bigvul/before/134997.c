void AppCacheUpdateJob::DiscardDuplicateResponses() {
  storage_->DoomResponses(manifest_url_, duplicate_response_ids_);
}
