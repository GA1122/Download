LocalFrame* FrameFetchContext::FrameOfImportsController() const {
  DCHECK(document_);
  DCHECK(!IsDetached());

  HTMLImportsController* imports_controller = document_->ImportsController();
  DCHECK(imports_controller);

  LocalFrame* frame = imports_controller->Master()->GetFrame();
  DCHECK(frame);
  return frame;
}
