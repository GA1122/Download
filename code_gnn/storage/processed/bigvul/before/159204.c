void DownloadItemImpl::UpdateValidatorsOnResumption(
    const DownloadCreateInfo& new_create_info) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK_EQ(RESUMING_INTERNAL, state_);
  DCHECK(!new_create_info.url_chain.empty());

  std::vector<GURL>::const_iterator chain_iter =
      new_create_info.url_chain.begin();
  if (*chain_iter == request_info_.url_chain.back())
    ++chain_iter;

  int origin_state = 0;
  bool is_partial = GetReceivedBytes() > 0;
  if (chain_iter != new_create_info.url_chain.end())
    origin_state |= ORIGIN_STATE_ON_RESUMPTION_ADDITIONAL_REDIRECTS;
  if (etag_ != new_create_info.etag ||
      last_modified_time_ != new_create_info.last_modified) {
    received_slices_.clear();
    destination_info_.received_bytes = 0;
    origin_state |= ORIGIN_STATE_ON_RESUMPTION_VALIDATORS_CHANGED;
  }
  if (content_disposition_ != new_create_info.content_disposition)
    origin_state |= ORIGIN_STATE_ON_RESUMPTION_CONTENT_DISPOSITION_CHANGED;
  RecordOriginStateOnResumption(
      is_partial, static_cast<OriginStateOnResumption>(origin_state));

  request_info_.url_chain.insert(request_info_.url_chain.end(), chain_iter,
                                 new_create_info.url_chain.end());
  etag_ = new_create_info.etag;
  last_modified_time_ = new_create_info.last_modified;
  response_headers_ = new_create_info.response_headers;
  content_disposition_ = new_create_info.content_disposition;
  mime_type_ = new_create_info.mime_type;

}
