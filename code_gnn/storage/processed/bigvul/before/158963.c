void PDFiumEngine::SelectionChangeInvalidator::Invalidate(
    const pp::Rect& selection) {
  pp::Rect expanded_selection = selection;
  expanded_selection.Inset(-1, -1);
  engine_->client_->Invalidate(expanded_selection);
}
