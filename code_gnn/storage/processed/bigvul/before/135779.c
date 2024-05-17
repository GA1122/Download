Document& SelectionController::GetDocument() const {
  DCHECK(frame_->GetDocument());
  return *frame_->GetDocument();
}
