void DataPipeConsumerDispatcher::UpdateSignalsStateNoLock() {
  lock_.AssertAcquired();

  const bool was_peer_closed = peer_closed_;
  const bool was_peer_remote = peer_remote_;
  size_t previous_bytes_available = bytes_available_;

  ports::PortStatus port_status;
  int rv = node_controller_->node()->GetStatus(control_port_, &port_status);
  peer_remote_ = rv == ports::OK && port_status.peer_remote;
  if (rv != ports::OK || !port_status.receiving_messages) {
    DVLOG(1) << "Data pipe consumer " << pipe_id_ << " is aware of peer closure"
             << " [control_port=" << control_port_.name() << "]";
    peer_closed_ = true;
  } else if (rv == ports::OK && port_status.has_messages && !in_transit_) {
    std::unique_ptr<ports::UserMessageEvent> message_event;
    do {
      int rv = node_controller_->node()->GetMessage(control_port_,
                                                    &message_event, nullptr);
      if (rv != ports::OK)
        peer_closed_ = true;
      if (message_event) {
        auto* message = message_event->GetMessage<UserMessageImpl>();
        if (message->user_payload_size() < sizeof(DataPipeControlMessage)) {
          peer_closed_ = true;
          break;
        }

        const DataPipeControlMessage* m =
            static_cast<const DataPipeControlMessage*>(message->user_payload());

        if (m->command != DataPipeCommand::DATA_WAS_WRITTEN) {
          DLOG(ERROR) << "Unexpected control message from producer.";
          peer_closed_ = true;
          break;
        }

        if (static_cast<size_t>(bytes_available_) + m->num_bytes >
            options_.capacity_num_bytes) {
          DLOG(ERROR) << "Producer claims to have written too many bytes.";
          peer_closed_ = true;
          break;
        }

        DVLOG(1) << "Data pipe consumer " << pipe_id_ << " is aware that "
                 << m->num_bytes << " bytes were written. [control_port="
                 << control_port_.name() << "]";

        bytes_available_ += m->num_bytes;
      }
    } while (message_event);
  }

  bool has_new_data = bytes_available_ != previous_bytes_available;
  if (has_new_data)
    new_data_available_ = true;

  if (peer_closed_ != was_peer_closed || has_new_data ||
      peer_remote_ != was_peer_remote) {
    watchers_.NotifyState(GetHandleSignalsStateNoLock());
  }
}
