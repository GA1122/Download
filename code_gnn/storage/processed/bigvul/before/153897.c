bool GLES2DecoderImpl::CheckResetStatus() {
  DCHECK(!WasContextLost());
  DCHECK(context_->IsCurrent(nullptr));

  if (IsRobustnessSupported()) {
    GLenum driver_status = api()->glGetGraphicsResetStatusARBFn();
    if (driver_status == GL_NO_ERROR)
      return false;

    LOG(ERROR) << (surface_->IsOffscreen() ? "Offscreen" : "Onscreen")
               << " context lost via ARB/EXT_robustness. Reset status = "
               << GLES2Util::GetStringEnum(driver_status);

    if (workarounds().use_virtualized_gl_contexts)
      driver_status = GL_UNKNOWN_CONTEXT_RESET_ARB;

    switch (driver_status) {
      case GL_GUILTY_CONTEXT_RESET_ARB:
        MarkContextLost(error::kGuilty);
        break;
      case GL_INNOCENT_CONTEXT_RESET_ARB:
        MarkContextLost(error::kInnocent);
        break;
      case GL_UNKNOWN_CONTEXT_RESET_ARB:
        MarkContextLost(error::kUnknown);
        break;
      default:
        NOTREACHED();
        return false;
    }
    reset_by_robustness_extension_ = true;
    return true;
  }
  return false;
}
