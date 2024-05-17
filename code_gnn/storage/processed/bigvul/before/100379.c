TransportDIB* BrowserRenderProcessHost::MapTransportDIB(
    TransportDIB::Id dib_id) {
#if defined(OS_WIN)
  HANDLE section = win_util::GetSectionFromProcess(
      dib_id.handle, GetRendererProcessHandle(), false  );
  return TransportDIB::Map(section);
#elif defined(OS_MACOSX)
  return widget_helper_->MapTransportDIB(dib_id);
#elif defined(OS_LINUX)
  return TransportDIB::Map(dib_id);
#endif   
}
