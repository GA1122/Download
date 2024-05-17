AudioHandler::~AudioHandler() {
  DCHECK(IsMainThread());
  DCHECK(!GetNode());
  InstanceCounters::DecrementCounter(InstanceCounters::kAudioHandlerCounter);
#if DEBUG_AUDIONODE_REFERENCES
  --node_count_[GetNodeType()];
  fprintf(stderr, "[%16p]: %16p: %2d: AudioHandler::~AudioHandler() %d [%d]\n",
          Context(), this, GetNodeType(), connection_ref_count_,
          node_count_[GetNodeType()]);
#endif
}
