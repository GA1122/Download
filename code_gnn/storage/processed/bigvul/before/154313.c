bool GLES2DecoderImpl::MakeCurrent() {
  DCHECK(surface_);
  if (!context_.get())
    return false;

  if (WasContextLost()) {
    LOG(ERROR) << "  GLES2DecoderImpl: Trying to make lost context current.";
    return false;
  }

  if (!context_->MakeCurrent(surface_.get())) {
    LOG(ERROR) << "  GLES2DecoderImpl: Context lost during MakeCurrent.";
    MarkContextLost(error::kMakeCurrentFailed);
    group_->LoseContexts(error::kUnknown);
    return false;
  }
  DCHECK_EQ(api(), gl::g_current_gl_context);

  if (CheckResetStatus()) {
    LOG(ERROR)
        << "  GLES2DecoderImpl: Context reset detected after MakeCurrent.";
    group_->LoseContexts(error::kUnknown);
    return false;
  }

  ProcessFinishedAsyncTransfers();

  if (workarounds().unbind_fbo_on_context_switch)
    RestoreFramebufferBindings();

  framebuffer_state_.clear_state_dirty = true;
  state_.stencil_state_changed_since_validation = true;

  RestoreAllExternalTextureBindingsIfNeeded();

  texture_refs_pending_destruction_.clear();

  return true;
}
