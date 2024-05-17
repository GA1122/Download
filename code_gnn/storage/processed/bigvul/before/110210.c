void NaClProcessHost::Launch(
    ChromeRenderMessageFilter* chrome_render_message_filter,
    int socket_count,
    IPC::Message* reply_msg,
    scoped_refptr<ExtensionInfoMap> extension_info_map) {
  chrome_render_message_filter_ = chrome_render_message_filter;
  reply_msg_ = reply_msg;
  extension_info_map_ = extension_info_map;

  if (socket_count > 8) {
    delete this;
    return;
  }

  NaClBrowser* nacl_browser = NaClBrowser::GetInstance();
  nacl_browser->EnsureAllResourcesAvailable();
  if (!nacl_browser->IsOk()) {
    DLOG(ERROR) << "Cannot launch NaCl process";
    delete this;
    return;
  }


  for (int i = 0; i < socket_count; i++) {
    nacl::Handle pair[2];
    if (nacl::SocketPair(pair) == -1) {
      delete this;
      return;
    }
    internal_->sockets_for_renderer.push_back(pair[0]);
    internal_->sockets_for_sel_ldr.push_back(pair[1]);
    SetCloseOnExec(pair[0]);
    SetCloseOnExec(pair[1]);
  }

  if (!LaunchSelLdr()) {
    delete this;
  }
}
