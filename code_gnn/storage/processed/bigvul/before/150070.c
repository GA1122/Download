void LayerTreeHostImpl::UpdateAnimationState(bool start_ready_animations) {
  std::unique_ptr<MutatorEvents> events = mutator_host_->CreateEvents();

  const bool has_active_animations =
      mutator_host_->UpdateAnimationState(start_ready_animations, events.get());

  if (!events->IsEmpty())
    client_->PostAnimationEventsToMainThreadOnImplThread(std::move(events));

  if (has_active_animations)
    SetNeedsOneBeginImplFrame();
}
