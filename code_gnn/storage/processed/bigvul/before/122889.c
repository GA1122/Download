TransportDIB* RenderProcessHostImpl::MapTransportDIB(
    TransportDIB::Id dib_id) {
#if defined(OS_WIN)
  HANDLE section;
  DuplicateHandle(GetHandle(), dib_id.handle, GetCurrentProcess(), &section,
                  STANDARD_RIGHTS_REQUIRED | FILE_MAP_READ | FILE_MAP_WRITE,
                  FALSE, 0);
  return TransportDIB::Map(section);
#elif defined(OS_MACOSX)
  return widget_helper_->MapTransportDIB(dib_id);
#elif defined(OS_ANDROID)
  return TransportDIB::Map(dib_id);
#elif defined(OS_POSIX)
  return TransportDIB::Map(dib_id.shmkey);
#endif   
}
