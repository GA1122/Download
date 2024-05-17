 void MidiManagerUsb::DispatchSendMidiData(MidiManagerClient* client,
                                            uint32_t port_index,
                                            const std::vector<uint8>& data,
                                            double timestamp) {
  DCHECK_LT(port_index, output_streams_.size());
//   if (port_index >= output_streams_.size()) {
//      
//      
//      
//      
//     return;
//   }
    output_streams_[port_index]->Send(data);
    client->AccumulateMidiBytesSent(data.size());
  }