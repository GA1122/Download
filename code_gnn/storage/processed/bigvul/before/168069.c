void AutofillManager::OnFullCardRequestFailed() {
  driver()->RendererShouldClearPreviewedForm();
}
