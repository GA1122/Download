bool WorkerFetchContext::ShouldBlockRequestByInspector(const KURL& url) const {
  bool should_block_request = false;
  probe::shouldBlockRequest(global_scope_, url, &should_block_request);
  return should_block_request;
}
