void AudioNode::DisconnectAllFromOutput(unsigned output_index) {
  Handler().Output(output_index).DisconnectAll();
  connected_nodes_[output_index] = nullptr;
  connected_params_[output_index] = nullptr;
}
