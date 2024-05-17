void FocusNotifier(WebPluginDelegateImpl *instance) {
  uint32 process_id = getpid();
  uint32 instance_id = reinterpret_cast<uint32>(instance);
  PluginThread* plugin_thread = PluginThread::current();
  if (plugin_thread) {
    plugin_thread->Send(
        new PluginProcessHostMsg_PluginReceivedFocus(process_id, instance_id));
  }
}
