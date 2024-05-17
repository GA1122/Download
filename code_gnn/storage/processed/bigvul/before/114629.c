void WebPluginDelegateProxy::ImeCompositionCompleted(const string16& text,
                                                     int plugin_id) {
  if (instance_id_ != plugin_id)
    return;

  IPC::Message* msg = new PluginMsg_ImeCompositionCompleted(instance_id_, text);
  msg->set_unblock(true);
  Send(msg);
}
