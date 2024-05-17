bool DelegatedFrameHost::HasSavedFrame() {
  return frame_evictor_->HasFrame();
}
