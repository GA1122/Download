void AudioNode::DidAddOutput(unsigned number_of_outputs) {
  connected_nodes_.push_back(nullptr);
  DCHECK_EQ(number_of_outputs, connected_nodes_.size());
  connected_params_.push_back(nullptr);
  DCHECK_EQ(number_of_outputs, connected_params_.size());
}
