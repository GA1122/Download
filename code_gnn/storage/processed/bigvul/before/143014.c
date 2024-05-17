void BaseAudioContext::NotifySourceNodeStartedProcessing(AudioNode* node) {
  DCHECK(IsMainThread());
  GraphAutoLocker locker(this);

  active_source_nodes_.push_back(node);
  node->Handler().MakeConnection();
}
