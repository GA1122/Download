void DocumentLoader::DidCommitNavigation(
    WebGlobalObjectReusePolicy global_object_reuse_policy) {
  if (GetFrameLoader().StateMachine()->CreatingInitialEmptyDocument())
    return;

  if (!frame_->Loader().StateMachine()->CommittedMultipleRealLoads() &&
      load_type_ == WebFrameLoadType::kStandard) {
    frame_->Loader().StateMachine()->AdvanceTo(
        FrameLoaderStateMachine::kCommittedMultipleRealLoads);
  }

  WebHistoryCommitType commit_type = LoadTypeToCommitType(load_type_);
  frame_->GetFrameScheduler()->DidCommitProvisionalLoad(
      commit_type == kWebHistoryInertCommit,
      load_type_ == WebFrameLoadType::kReload, frame_->IsLocalRoot());
  frame_->ResumeSubresourceLoading();
  GetLocalFrameClient().DispatchDidCommitLoad(history_item_.Get(), commit_type,
                                              global_object_reuse_policy);

  frame_->GetSecurityContext()
      ->GetContentSecurityPolicy()
      ->ReportAccumulatedHeaders(&GetLocalFrameClient());

  if (service_worker_network_provider_ &&
      service_worker_network_provider_->IsControlledByServiceWorker() ==
          blink::mojom::ControllerServiceWorkerMode::kControlled) {
    GetLocalFrameClient().DidObserveLoadingBehavior(
        kWebLoadingBehaviorServiceWorkerControlled);
  }

  Document* document = frame_->GetDocument();
  InteractiveDetector* interactive_detector =
      InteractiveDetector::From(*document);
  if (interactive_detector)
    interactive_detector->SetNavigationStartTime(GetTiming().NavigationStart());

  TRACE_EVENT1("devtools.timeline", "CommitLoad", "data",
               inspector_commit_load_event::Data(frame_));

  probe::didCommitLoad(frame_, this);

  DispatchLinkHeaderPreloads(nullptr, LinkLoader::kOnlyLoadNonMedia);

  frame_->GetPage()->DidCommitLoad(frame_);
  GetUseCounter().DidCommitLoad(frame_);

  if (response_.IsLegacySymantecCert()) {
    UseCounter::Count(
        this, frame_->Tree().Parent()
                  ? WebFeature::kLegacySymantecCertInSubframeMainResource
                  : WebFeature::kLegacySymantecCertMainFrameResource);
    GetLocalFrameClient().ReportLegacySymantecCert(response_.Url(),
                                                   false  );
  }

  if (response_.IsLegacyTLSVersion()) {
    UseCounter::Count(this,
                      frame_->Tree().Parent()
                          ? WebFeature::kLegacyTLSVersionInSubframeMainResource
                          : WebFeature::kLegacyTLSVersionInMainFrameResource);
    GetLocalFrameClient().ReportLegacyTLSVersion(response_.Url());
  }
}
