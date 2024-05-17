void RenderThread::ClearHostResolverCache() {
  int rv;
  Send(new ViewHostMsg_ClearHostResolverCache(&rv));
}
