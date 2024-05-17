void RenderThread::CloseIdleConnections() {
  Send(new ViewHostMsg_CloseIdleConnections());
}
