void WebPluginProxy::StartIme() {
  IPC::Message* msg = new PluginHostMsg_StartIme(route_id_);
  msg->set_unblock(true);
  Send(msg);
}
