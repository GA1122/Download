RenderProcessImpl::~RenderProcessImpl() {
#ifndef NDEBUG
  webkit_glue::CheckForLeaks();
#endif

  GetShutDownEvent()->Signal();
  ClearTransportDIBCache();
}
