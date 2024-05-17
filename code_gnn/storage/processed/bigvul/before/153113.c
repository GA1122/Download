PDFiumEngine::SelectionChangeInvalidator::SelectionChangeInvalidator(
    PDFiumEngine* engine) : engine_(engine) {
  previous_origin_ = engine_->GetVisibleRect().point();
  GetVisibleSelectionsScreenRects(&old_selections_);
}
