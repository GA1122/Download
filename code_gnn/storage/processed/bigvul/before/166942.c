void BaseAudioContext::Uninitialize() {
  DCHECK(IsMainThread());

  if (!IsDestinationInitialized())
    return;

  if (destination_node_)
    destination_node_->Handler().Uninitialize();

  GetDeferredTaskHandler().FinishTailProcessing();

  ReleaseActiveSourceNodes();

  RejectPendingResolvers();
  DidClose();

  DCHECK(listener_);
  listener_->WaitForHRTFDatabaseLoaderThreadCompletion();

  RecordAutoplayStatus();

  Clear();
}
