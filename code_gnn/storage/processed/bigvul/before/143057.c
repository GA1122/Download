AudioDestinationNode* BaseAudioContext::destination() const {
  DCHECK(!IsAudioThread());
  return destination_node_;
}
