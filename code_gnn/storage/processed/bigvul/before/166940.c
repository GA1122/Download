void BaseAudioContext::StartRendering() {
  DCHECK(IsMainThread());
  DCHECK(destination_node_);
  DCHECK(IsAllowedToStart());

  if (context_state_ == kSuspended) {
    destination()->GetAudioDestinationHandler().StartRendering();
  }
}
