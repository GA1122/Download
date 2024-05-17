void WebPluginDelegateProxy::SetWindowFocus(bool window_has_focus) {
  IPC::Message* msg = new PluginMsg_SetWindowFocus(instance_id_,
                                                   window_has_focus);
  msg->set_unblock(true);
  Send(msg);
}
