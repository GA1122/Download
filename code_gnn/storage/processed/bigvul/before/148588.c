void WebContentsImpl::UpdateUrlForUkmSource(ukm::UkmRecorder* service,
                                            ukm::SourceId ukm_source_id) {
  service->UpdateSourceURL(ukm_source_id, GetLastCommittedURL());
}
