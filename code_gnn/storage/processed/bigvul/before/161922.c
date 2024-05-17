bool PrintRenderFrameHelper::PrintPreviewContext::IsModifiable() const {
  DCHECK(state_ != UNINITIALIZED);
  return is_modifiable_;
}
