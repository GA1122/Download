static void ReleaseTransportDIB(TransportDIB* dib) {
  if (dib) {
    IPC::Message* message = new ViewHostMsg_FreeTransportDIB(dib->id());
    RenderThread::current()->Send(message);
  }
}
