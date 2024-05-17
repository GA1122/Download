void MessageService::CloseChannel(int port_id, bool connection_error) {
  int channel_id = GET_CHANNEL_ID(port_id);
  MessageChannelMap::iterator it = channels_.find(channel_id);
  if (it == channels_.end()) {
    PendingChannelMap::iterator pending = pending_channels_.find(channel_id);
    if (pending != pending_channels_.end()) {
      lazy_background_task_queue_->AddPendingTask(
          pending->second.first, pending->second.second,
          base::Bind(&MessageService::PendingCloseChannel,
                     weak_factory_.GetWeakPtr(), port_id, connection_error));
    }
    return;
  }
  CloseChannelImpl(it, port_id, connection_error, true);
}
