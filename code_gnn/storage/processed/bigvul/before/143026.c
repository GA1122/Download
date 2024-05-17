void BaseAudioContext::Uninitialize() {
  DCHECK(IsMainThread());

  if (!IsDestinationInitialized())
    return;

  if (destination_node_)
    destination_node_->Handler().Uninitialize();

  GetDeferredTaskHandler().FinishTailProcessing();

  ReleaseActiveSourceNodes();

  RejectPendingResolvers();

  DCHECK(listener_);
  listener_->WaitForHRTFDatabaseLoaderThreadCompletion();

  Clear();

  DCHECK(!is_resolving_resume_promises_);
  DCHECK_EQ(resume_resolvers_.size(), 0u);
   DCHECK_EQ(active_source_nodes_.size(), 0u);
 }
