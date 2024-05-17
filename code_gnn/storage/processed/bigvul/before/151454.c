bool FrameFetchContext::ShouldBlockRequestByInspector(const KURL& url) const {
  if (IsDetached())
    return false;
  bool should_block_request = false;
  probe::shouldBlockRequest(GetFrame()->GetDocument(), url,
                            &should_block_request);
  return should_block_request;
}
