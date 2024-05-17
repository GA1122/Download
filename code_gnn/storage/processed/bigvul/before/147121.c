void DocumentLoader::DidCommitNavigation() {
  if (GetFrameLoader().StateMachine()->CreatingInitialEmptyDocument())
    return;

  if (!frame_->Loader().StateMachine()->CommittedMultipleRealLoads() &&
      load_type_ == kFrameLoadTypeStandard) {
    frame_->Loader().StateMachine()->AdvanceTo(
        FrameLoaderStateMachine::kCommittedMultipleRealLoads);
  }

  HistoryCommitType commit_type = LoadTypeToCommitType(load_type_);
  frame_->FrameScheduler()->DidCommitProvisionalLoad(
      commit_type == kHistoryInertCommit, load_type_ == kFrameLoadTypeReload,
      frame_->IsLocalRoot());
  GetLocalFrameClient().DispatchDidCommitLoad(history_item_.Get(), commit_type);

  frame_->GetSecurityContext()
      ->GetContentSecurityPolicy()
      ->ReportAccumulatedHeaders(&GetLocalFrameClient());

  if (service_worker_network_provider_ &&
      service_worker_network_provider_->IsControlledByServiceWorker()) {
    GetLocalFrameClient().DidObserveLoadingBehavior(
        kWebLoadingBehaviorServiceWorkerControlled);
  }

  DispatchLinkHeaderPreloads(nullptr, LinkLoader::kOnlyLoadNonMedia);

  TRACE_EVENT1("devtools.timeline", "CommitLoad", "data",
               InspectorCommitLoadEvent::Data(frame_));
  probe::didCommitLoad(frame_, this);
  frame_->GetPage()->DidCommitLoad(frame_);
}
