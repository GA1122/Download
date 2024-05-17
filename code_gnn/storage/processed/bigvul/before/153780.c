void GLES2Implementation::OnGpuControlLostContextMaybeReentrant() {
  share_group_->Lose();
}
