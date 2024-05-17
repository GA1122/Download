WGC3Denum convertReason(ContentGLContext::ContextLostReason reason) {
  switch (reason) {
  case ContentGLContext::kGuilty:
    return GL_GUILTY_CONTEXT_RESET_ARB;
  case ContentGLContext::kInnocent:
    return GL_INNOCENT_CONTEXT_RESET_ARB;
  case ContentGLContext::kUnknown:
    return GL_UNKNOWN_CONTEXT_RESET_ARB;
  }

  NOTREACHED();
  return GL_UNKNOWN_CONTEXT_RESET_ARB;
}
