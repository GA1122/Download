void DelegatedFrameHost::WasHidden() {
  frame_evictor_->SetVisible(false);
  released_front_lock_ = NULL;
}
