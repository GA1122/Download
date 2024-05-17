void AudioHandler::BreakConnectionWithLock() {
  AtomicDecrement(&connection_ref_count_);

#if DEBUG_AUDIONODE_REFERENCES
  fprintf(stderr, "[%16p]: %16p: %2d: AudioHandler::deref %3d [%3d]\n",
          Context(), this, GetNodeType(), connection_ref_count_,
          node_count_[GetNodeType()]);
#endif

  if (!connection_ref_count_)
    DisableOutputsIfNecessary();
}
