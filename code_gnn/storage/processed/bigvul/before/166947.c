BaseAudioContext::~BaseAudioContext() {
  GetDeferredTaskHandler().ContextWillBeDestroyed();
  DCHECK(!active_source_nodes_.size());
  DCHECK(!is_resolving_resume_promises_);
  DCHECK(!resume_resolvers_.size());
  DCHECK(!autoplay_status_.has_value());
}
