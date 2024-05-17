void FrameLoader::Init() {
  ScriptForbiddenScope forbid_scripts;

  ResourceRequest initial_request{KURL(g_empty_string)};
  initial_request.SetRequestContext(WebURLRequest::kRequestContextInternal);
  initial_request.SetFrameType(
      frame_->IsMainFrame() ? network::mojom::RequestContextFrameType::kTopLevel
                            : network::mojom::RequestContextFrameType::kNested);

  provisional_document_loader_ =
      Client()->CreateDocumentLoader(frame_, initial_request, SubstituteData(),
                                     ClientRedirectPolicy::kNotClientRedirect,
                                     base::UnguessableToken::Create());
  provisional_document_loader_->StartLoading();

  frame_->GetDocument()->CancelParsing();

  state_machine_.AdvanceTo(
      FrameLoaderStateMachine::kDisplayingInitialEmptyDocument);

  document_loader_->SetSentDidFinishLoad();
  if (frame_->GetPage()->Paused())
    SetDefersLoading(true);

  TakeObjectSnapshot();
}
