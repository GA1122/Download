WGC3Denum convertReason(gpu::error::ContextLostReason reason) {
  switch (reason) {
  case gpu::error::kGuilty:
    return GL_GUILTY_CONTEXT_RESET_ARB;
  case gpu::error::kInnocent:
    return GL_INNOCENT_CONTEXT_RESET_ARB;
  case gpu::error::kUnknown:
    return GL_UNKNOWN_CONTEXT_RESET_ARB;
  }

  NOTREACHED();
  return GL_UNKNOWN_CONTEXT_RESET_ARB;
}
