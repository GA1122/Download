void DataPipeConsumerDispatcher::NotifyRead(uint32_t num_bytes) {
  DVLOG(1) << "Data pipe consumer " << pipe_id_
           << " notifying peer: " << num_bytes
           << " bytes read. [control_port=" << control_port_.name() << "]";

  SendDataPipeControlMessage(node_controller_, control_port_,
                             DataPipeCommand::DATA_WAS_READ, num_bytes);
}
