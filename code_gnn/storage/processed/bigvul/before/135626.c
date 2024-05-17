void FrameSelection::ContextDestroyed(Document* document) {
  granularity_ = TextGranularity::kCharacter;

  layout_selection_->OnDocumentShutdown();

  frame_->GetEditor().ClearTypingStyle();
}
