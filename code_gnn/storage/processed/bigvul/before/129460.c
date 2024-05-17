bool GLES2DecoderImpl::MakeCurrent() {
  if (!context_.get())
    return false;

  if (!context_->MakeCurrent(surface_.get()) || WasContextLost()) {
    LOG(ERROR) << "  GLES2DecoderImpl: Context lost during MakeCurrent.";

    if (workarounds().exit_on_context_lost) {
      LOG(ERROR) << "Exiting GPU process because some drivers cannot reset"
                 << " a D3D device in the Chrome GPU process sandbox.";
#if defined(OS_WIN)
      base::win::SetShouldCrashOnProcessDetach(false);
#endif
      exit(0);
    }

    return false;
  }

  ProcessFinishedAsyncTransfers();

  if (workarounds().unbind_fbo_on_context_switch)
    RestoreFramebufferBindings();

  framebuffer_state_.clear_state_dirty = true;

  return true;
}
