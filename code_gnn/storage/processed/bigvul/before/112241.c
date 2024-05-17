HostPortAllocatorSession::~HostPortAllocatorSession() {
  STLDeleteElements(&url_fetchers_);
}
