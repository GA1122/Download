void RenderProcessImpl::FreeTransportDIB(TransportDIB* dib) {
  if (!dib)
    return;

#if defined(OS_MACOSX)
  IPC::Message* msg = new ViewHostMsg_FreeTransportDIB(dib->id());
  main_thread()->Send(msg);
#endif

  delete dib;
}
