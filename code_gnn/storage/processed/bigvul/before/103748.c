void RenderThread::EnableSpdy(bool enable) {
  Send(new ViewHostMsg_EnableSpdy(enable));
}
