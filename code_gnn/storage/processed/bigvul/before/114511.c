void PluginChannel::NotifyRenderersOfPendingShutdown() {
  Broadcast(new PluginHostMsg_PluginShuttingDown());
 }
