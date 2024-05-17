void FrameLoader::Init() {
  ScriptForbiddenScope forbid_scripts;

  auto navigation_params = std::make_unique<WebNavigationParams>();
  navigation_params->url = KURL(g_empty_string);
  provisional_document_loader_ = Client()->CreateDocumentLoader(
      frame_, kWebNavigationTypeOther, std::move(navigation_params),
      nullptr  );
  provisional_document_loader_->StartLoading();

  frame_->GetDocument()->CancelParsing();

  state_machine_.AdvanceTo(
      FrameLoaderStateMachine::kDisplayingInitialEmptyDocument);

  document_loader_->SetSentDidFinishLoad();
  if (frame_->GetPage()->Paused())
    frame_->SetLifecycleState(mojom::FrameLifecycleState::kPaused);

  TakeObjectSnapshot();
}
