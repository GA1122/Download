 void FrameLoader::Init() {
  ScriptForbiddenScope forbid_scripts;

  ResourceRequest initial_request{KURL(g_empty_string)};
  initial_request.SetRequestContext(mojom::RequestContextType::INTERNAL);
  initial_request.SetFrameType(
      frame_->IsMainFrame() ? network::mojom::RequestContextFrameType::kTopLevel
                            : network::mojom::RequestContextFrameType::kNested);
  initial_request.SetHasUserGesture(
      LocalFrame::HasTransientUserActivation(frame_));

  provisional_document_loader_ = CreateDocumentLoader(
      initial_request, SubstituteData(),
      ClientRedirectPolicy::kNotClientRedirect,
      base::UnguessableToken::Create(), WebFrameLoadType::kStandard,
      kWebNavigationTypeOther, nullptr  ,
      nullptr  );
  provisional_document_loader_->StartLoading();

  frame_->GetDocument()->CancelParsing();

  state_machine_.AdvanceTo(
      FrameLoaderStateMachine::kDisplayingInitialEmptyDocument);

  document_loader_->SetSentDidFinishLoad();
  if (frame_->GetPage()->Paused())
    SetDefersLoading(true);

  TakeObjectSnapshot();
}
