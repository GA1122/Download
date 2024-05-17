void PDFiumEngine::SetSelecting(bool selecting) {
  bool was_selecting = selecting_;
  selecting_ = selecting;
  if (selecting_ != was_selecting)
    client_->IsSelectingChanged(selecting);
}
