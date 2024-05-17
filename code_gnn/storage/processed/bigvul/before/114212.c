GpuMessageFilter::~GpuMessageFilter() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
}
