MediaInterfaceProxy::~MediaInterfaceProxy() {
  DVLOG(1) << __FUNCTION__;
  DCHECK(thread_checker_.CalledOnValidThread());
}
