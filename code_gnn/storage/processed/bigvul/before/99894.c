 void WebPluginDelegateProxy::SetFocus() {
  Send(new PluginMsg_SetFocus(instance_id_));
}
