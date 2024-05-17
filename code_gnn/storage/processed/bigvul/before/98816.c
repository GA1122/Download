void WebPluginDelegateProxy::SetContentAreaFocus(bool has_focus) {
  IPC::Message* msg = new PluginMsg_SetContentAreaFocus(instance_id_,
                                                        has_focus);
  msg->set_unblock(true);
  Send(msg);
}
