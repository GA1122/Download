NavigationURLLoaderImpl::~NavigationURLLoaderImpl() {
  BrowserThread::DeleteSoon(BrowserThread::IO, FROM_HERE,
                            request_controller_.release());
}
