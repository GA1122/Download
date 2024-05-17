void RenderThread::CloseCurrentConnections() {
  Send(new ViewHostMsg_CloseCurrentConnections());
}
