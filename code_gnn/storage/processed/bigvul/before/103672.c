TransportDIB* BrowserRenderProcessHost::MapTransportDIB(
    TransportDIB::Id dib_id) {
#if defined(OS_WIN)
  HANDLE section = chrome::GetSectionFromProcess(
      dib_id.handle, GetHandle(), false  );
  return TransportDIB::Map(section);
#elif defined(OS_MACOSX)
  return widget_helper_->MapTransportDIB(dib_id);
#elif defined(OS_POSIX)
  return TransportDIB::Map(dib_id.shmkey);
#endif   
}
