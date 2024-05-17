TransportDIB* RenderProcessImpl::CreateTransportDIB(size_t size) {
#if defined(OS_WIN) || defined(OS_LINUX) || \
    defined(OS_OPENBSD) || defined(OS_ANDROID)
  return TransportDIB::Create(size, transport_dib_next_sequence_number_++);
#elif defined(OS_MACOSX)
  TransportDIB::Handle handle;
  IPC::Message* msg = new ViewHostMsg_AllocTransportDIB(size, true, &handle);
  if (!main_thread()->Send(msg))
    return NULL;
  if (handle.fd < 0)
    return NULL;
  return TransportDIB::Map(handle);
#endif   
}
