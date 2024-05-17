void WebPluginDelegateProxy::ImeCompositionUpdated(
    const string16& text,
    const std::vector<int>& clauses,
    const std::vector<int>& target,
    int cursor_position,
    int plugin_id) {
  if (instance_id_ != plugin_id)
    return;

  IPC::Message* msg = new PluginMsg_ImeCompositionUpdated(instance_id_,
      text, clauses, target, cursor_position);
  msg->set_unblock(true);
  Send(msg);
}
