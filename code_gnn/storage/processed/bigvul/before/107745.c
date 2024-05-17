void FrameLoader::DidExplicitOpen() {
  probe::LifecycleEvent(frame_, GetDocumentLoader(), "init",
                        CurrentTimeTicksInSeconds());
  if (!state_machine_.CommittedFirstRealDocumentLoad())
    state_machine_.AdvanceTo(FrameLoaderStateMachine::kCommittedFirstRealLoad);

  if (Frame* parent = frame_->Tree().Parent()) {
    auto* parent_local_frame = DynamicTo<LocalFrame>(parent);
    if ((parent_local_frame &&
         parent_local_frame->GetDocument()->LoadEventStillNeeded()) ||
        (parent->IsRemoteFrame() && parent->IsLoading())) {
      progress_tracker_->ProgressStarted();
    }
  }
}
