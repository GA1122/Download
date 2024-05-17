void AudioHandler::SetNodeType(NodeType type) {
  DCHECK_EQ(node_type_, kNodeTypeUnknown);
  DCHECK_NE(type, kNodeTypeUnknown);
  DCHECK_NE(type, kNodeTypeEnd);

  node_type_ = type;

#if DEBUG_AUDIONODE_REFERENCES
  ++node_count_[type];
  fprintf(stderr, "[%16p]: %16p: %2d: AudioHandler::AudioHandler [%3d]\n",
          Context(), this, GetNodeType(), node_count_[GetNodeType()]);
#endif
}
