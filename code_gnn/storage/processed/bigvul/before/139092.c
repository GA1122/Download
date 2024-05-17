AudioRendererHost::~AudioRendererHost() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(delegates_.empty());
}
