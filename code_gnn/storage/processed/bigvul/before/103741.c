void RenderThread::ClearCache(bool preserve_ssl_host_info) {
  int rv;
  Send(new ViewHostMsg_ClearCache(preserve_ssl_host_info, &rv));
}
