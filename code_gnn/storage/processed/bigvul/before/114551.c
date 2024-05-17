void WebPluginProxy::ReportExecutableMemory(size_t size) {
  PluginThread::current()->Send(
      new PluginProcessHostMsg_ReportExecutableMemory(size));
}
