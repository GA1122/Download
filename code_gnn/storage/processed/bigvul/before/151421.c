LocalFrame* FrameFetchContext::GetFrame() const {
  DCHECK(!IsDetached());

  if (!document_loader_)
    return FrameOfImportsController();

  LocalFrame* frame = document_loader_->GetFrame();
  DCHECK(frame);
  return frame;
}
