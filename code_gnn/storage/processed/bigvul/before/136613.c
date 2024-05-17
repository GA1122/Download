void DocumentLoader::InstallNewDocument(
    const KURL& url,
    Document* owner_document,
    WebGlobalObjectReusePolicy global_object_reuse_policy,
    const AtomicString& mime_type,
    const AtomicString& encoding,
    InstallNewDocumentReason reason,
    ParserSynchronizationPolicy parsing_policy,
    const KURL& overriding_url) {
  DCHECK(!frame_->GetDocument() || !frame_->GetDocument()->IsActive());
  DCHECK_EQ(frame_->Tree().ChildCount(), 0u);
  if (GetFrameLoader().StateMachine()->IsDisplayingInitialEmptyDocument()) {
    GetFrameLoader().StateMachine()->AdvanceTo(
        FrameLoaderStateMachine::kCommittedFirstRealLoad);
  }

  const SecurityOrigin* previous_security_origin = nullptr;
  if (frame_->GetDocument())
    previous_security_origin = frame_->GetDocument()->GetSecurityOrigin();

  if (global_object_reuse_policy != WebGlobalObjectReusePolicy::kUseExisting)
    frame_->SetDOMWindow(LocalDOMWindow::Create(*frame_));

  if (reason == InstallNewDocumentReason::kNavigation)
    WillCommitNavigation();

  Document* document = frame_->DomWindow()->InstallNewDocument(
      mime_type,
      DocumentInit::Create()
          .WithFrame(frame_)
          .WithURL(url)
          .WithOwnerDocument(owner_document)
          .WithNewRegistrationContext(),
      false);

  if (frame_->IsMainFrame())
    frame_->ClearActivation();

  if (frame_->HasReceivedUserGestureBeforeNavigation() != user_activated_) {
    frame_->SetDocumentHasReceivedUserGestureBeforeNavigation(user_activated_);
    GetLocalFrameClient().SetHasReceivedUserGestureBeforeNavigation(
        user_activated_);
  }

  if (ShouldClearWindowName(*frame_, previous_security_origin, *document)) {
    frame_->Tree().ExperimentalSetNulledName();
  }

  if (!overriding_url.IsEmpty())
    document->SetBaseURLOverride(overriding_url);
  DidInstallNewDocument(document);

  if (reason == InstallNewDocumentReason::kNavigation)
    DidCommitNavigation(global_object_reuse_policy);

  if (document->GetSettings()
          ->GetForceTouchEventFeatureDetectionForInspector()) {
    OriginTrialContext::FromOrCreate(document)->AddFeature(
        "ForceTouchEventFeatureDetectionForInspector");
  }
  OriginTrialContext::AddTokensFromHeader(
      document, response_.HttpHeaderField(HTTPNames::Origin_Trial));

  parser_ = document->OpenForNavigation(parsing_policy, mime_type, encoding);

  document->ApplyFeaturePolicyFromHeader(
      RuntimeEnabledFeatures::FeaturePolicyEnabled()
          ? response_.HttpHeaderField(HTTPNames::Feature_Policy)
          : g_empty_string);

  GetFrameLoader().DispatchDidClearDocumentOfWindowObject();
}
