std::unique_ptr<DocumentState> BuildDocumentState() {
  std::unique_ptr<DocumentState> document_state =
      std::make_unique<DocumentState>();
  InternalDocumentStateData::FromDocumentState(document_state.get())
      ->set_navigation_state(NavigationState::CreateContentInitiated());
  return document_state;
}
