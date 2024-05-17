void AudioNode::SetHandler(scoped_refptr<AudioHandler> handler) {
  DCHECK(handler);
  handler_ = std::move(handler);

#if DEBUG_AUDIONODE_REFERENCES
  fprintf(stderr, "[%16p]: %16p: %2d: AudioNode::AudioNode %16p\n", context(),
          this, handler_->GetNodeType(), handler_.get());
#endif
}
