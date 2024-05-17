void BrowserPluginGuest::RenderViewGone(base::TerminationStatus status) {
  if (pending_input_event_reply_.get()) {
    IPC::Message* reply_message = pending_input_event_reply_.release();
    BrowserPluginHostMsg_HandleInputEvent::WriteReplyParams(reply_message,
                                                            false);
    SendMessageToEmbedder(reply_message);
  }
  int process_id = web_contents()->GetRenderProcessHost()->GetID();
  SendMessageToEmbedder(new BrowserPluginMsg_GuestGone(embedder_routing_id(),
                                                       instance_id(),
                                                       process_id,
                                                       status));
  IDMap<RenderViewHost>::const_iterator iter(&pending_updates_);
  while (!iter.IsAtEnd()) {
    pending_updates_.Remove(iter.GetCurrentKey());
    iter.Advance();
  }

  switch (status) {
    case base::TERMINATION_STATUS_PROCESS_WAS_KILLED:
      RecordAction(UserMetricsAction("BrowserPlugin.Guest.Killed"));
      break;
    case base::TERMINATION_STATUS_PROCESS_CRASHED:
      RecordAction(UserMetricsAction("BrowserPlugin.Guest.Crashed"));
      break;
    case base::TERMINATION_STATUS_ABNORMAL_TERMINATION:
      RecordAction(UserMetricsAction("BrowserPlugin.Guest.AbnormalDeath"));
      break;
    default:
      break;
  }
}
