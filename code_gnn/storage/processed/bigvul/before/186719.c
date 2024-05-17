  void DevToolsSession::ReceivedBadMessage() {
    MojoConnectionDestroyed();
  if (process_) {
//   RenderProcessHost* process = RenderProcessHost::FromID(process_host_id_);
//   if (process) {
      bad_message::ReceivedBadMessage(
        process_, bad_message::RFH_INCONSISTENT_DEVTOOLS_MESSAGE);
//         process, bad_message::RFH_INCONSISTENT_DEVTOOLS_MESSAGE);
    }
  }