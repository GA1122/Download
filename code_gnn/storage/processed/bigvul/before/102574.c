bool NaClProcessHost::Launch(
    ChromeRenderMessageFilter* chrome_render_message_filter,
    int socket_count,
    IPC::Message* reply_msg) {
#ifdef DISABLE_NACL
  NOTIMPLEMENTED() << "Native Client disabled at build time";
  return false;
#else
  if (socket_count > 8) {
    return false;
  }


  for (int i = 0; i < socket_count; i++) {
    nacl::Handle pair[2];
    if (nacl::SocketPair(pair) == -1)
      return false;
    internal_->sockets_for_renderer.push_back(pair[0]);
    internal_->sockets_for_sel_ldr.push_back(pair[1]);
    SetCloseOnExec(pair[0]);
    SetCloseOnExec(pair[1]);
  }

  if (!LaunchSelLdr()) {
    return false;
  }
  chrome_render_message_filter_ = chrome_render_message_filter;
  reply_msg_ = reply_msg;

  return true;
#endif   
}
