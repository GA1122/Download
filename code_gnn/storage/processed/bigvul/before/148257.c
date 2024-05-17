PendingHostCreator::~PendingHostCreator() {
  connection_->Send(new PpapiHostMsg_CreateResourceHostsFromHostReply(
      routing_id_, sequence_id_, pending_resource_host_ids_));
}
