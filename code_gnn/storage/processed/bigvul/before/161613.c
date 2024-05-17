bool AudioNode::DisconnectFromOutputIfConnected(
    unsigned output_index,
    AudioNode& destination,
    unsigned input_index_of_destination) {
  AudioNodeOutput& output = Handler().Output(output_index);
  AudioNodeInput& input =
      destination.Handler().Input(input_index_of_destination);
  if (!output.IsConnectedToInput(input))
    return false;
  output.DisconnectInput(input);
  connected_nodes_[output_index]->erase(&destination);
  return true;
}
