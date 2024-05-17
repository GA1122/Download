void GLES2Implementation::OnGpuControlLostContext() {
  DCHECK(!lost_context_callback_run_);
  lost_context_callback_run_ = true;
  share_group_->Lose();
  if (!lost_context_callback_.is_null()) {
    std::move(lost_context_callback_).Run();
  }
}
