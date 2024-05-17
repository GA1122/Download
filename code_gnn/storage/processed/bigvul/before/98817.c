void WebPluginDelegateProxy::SetFocus(bool focused) {
  Send(new PluginMsg_SetFocus(instance_id_, focused));
}
