static void EnsureFrameLoaderHasCommitted(FrameLoader& frame_loader) {
  if (frame_loader.StateMachine()->CommittedMultipleRealLoads())
    return;
  frame_loader.StateMachine()->AdvanceTo(
      FrameLoaderStateMachine::kCommittedMultipleRealLoads);
}
