void DelegatedFrameHost::UnlockResources() {
  DCHECK(local_surface_id_.is_valid());
  frame_evictor_->UnlockFrame();
}
