void InspectorNetworkAgent::DidCommitLoad(LocalFrame* frame,
                                          DocumentLoader* loader) {
  DCHECK(inspected_frames_);
  DCHECK(IsMainThread());
  if (loader->GetFrame() != inspected_frames_->Root())
    return;

  if (state_->booleanProperty(NetworkAgentState::kCacheDisabled, false))
    GetMemoryCache()->EvictResources(MemoryCache::kDoNotEvictUnusedPreloads);

  resources_data_->Clear(IdentifiersFactory::LoaderId(loader));
}
