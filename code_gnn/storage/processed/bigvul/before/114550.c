void WebPluginProxy::ReparentPluginWindow(HWND window, HWND parent) {
  PluginThread::current()->Send(
      new PluginProcessHostMsg_ReparentPluginWindow(window, parent));
}
