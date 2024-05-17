void MessageService::PostMessage(
    int source_port_id, const std::string& message) {
  int channel_id = GET_CHANNEL_ID(source_port_id);
  MessageChannelMap::iterator iter = channels_.find(channel_id);
  if (iter == channels_.end()) {
    PendingChannelMap::iterator pending = pending_channels_.find(channel_id);
    if (pending != pending_channels_.end()) {
      lazy_background_task_queue_->AddPendingTask(
          pending->second.first, pending->second.second,
          base::Bind(&MessageService::PendingPostMessage,
                     weak_factory_.GetWeakPtr(), source_port_id, message));
    }
    return;
  }

  int dest_port_id = GET_OPPOSITE_PORT_ID(source_port_id);
  MessagePort* port = IS_OPENER_PORT_ID(dest_port_id) ?
      iter->second->opener.get() : iter->second->receiver.get();

  port->DispatchOnMessage(message, dest_port_id);
}
