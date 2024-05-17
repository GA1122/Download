void DocumentLoader::DidCommitNavigation(
    WebGlobalObjectReusePolicy global_object_reuse_policy) {
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
  GetLocalFrameClient().DispatchDidCommitLoad(history_item_.Get(), commit_type,
                                              global_object_reuse_policy);

  frame_->GetSecurityContext()
      ->GetContentSecurityPolicy()
      ->ReportAccumulatedHeaders(&GetLocalFrameClient());

  if (service_worker_network_provider_ &&
      service_worker_network_provider_->HasControllerServiceWorker()) {
    GetLocalFrameClient().DidObserveLoadingBehavior(
        kWebLoadingBehaviorServiceWorkerControlled);
  }

  DispatchLinkHeaderPreloads(nullptr, LinkLoader::kOnlyLoadNonMedia);

  Document* document = frame_->GetDocument();
  InteractiveDetector* interactive_detector =
      InteractiveDetector::From(*document);
  if (interactive_detector)
    interactive_detector->SetNavigationStartTime(GetTiming().NavigationStart());

  TRACE_EVENT1("devtools.timeline", "CommitLoad", "data",
               InspectorCommitLoadEvent::Data(frame_));
  probe::didCommitLoad(frame_, this);
  frame_->GetPage()->DidCommitLoad(frame_);

  if (response_.IsLegacySymantecCert()) {
    GetLocalFrameClient().ReportLegacySymantecCert(response_.Url());
  }
}
