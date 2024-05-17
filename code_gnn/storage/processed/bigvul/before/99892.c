static void ReleaseTransportDIB(TransportDIB *dib) {
  if (dib) {
    IPC::Message* msg = new ViewHostMsg_FreeTransportDIB(dib->id());
    RenderThread::current()->Send(msg);
  }
}
