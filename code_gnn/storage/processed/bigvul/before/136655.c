void FrameLoader::DidExplicitOpen() {
  if (!state_machine_.CommittedFirstRealDocumentLoad())
    state_machine_.AdvanceTo(FrameLoaderStateMachine::kCommittedFirstRealLoad);

  if (Frame* parent = frame_->Tree().Parent()) {
    if ((parent->IsLocalFrame() &&
         ToLocalFrame(parent)->GetDocument()->LoadEventStillNeeded()) ||
        (parent->IsRemoteFrame() && parent->IsLoading())) {
      progress_tracker_->ProgressStarted(document_loader_->LoadType());
    }
  }

  frame_->GetNavigationScheduler().Cancel();
}
