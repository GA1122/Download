void WebPluginDelegateProxy::InstallMissingPlugin() {
  Send(new PluginMsg_InstallMissingPlugin(instance_id_));
}
