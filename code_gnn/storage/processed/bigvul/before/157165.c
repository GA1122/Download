ResourceMultiBuffer* UrlData::multibuffer() {
  DCHECK(thread_checker_.CalledOnValidThread());
  return &multibuffer_;
}
