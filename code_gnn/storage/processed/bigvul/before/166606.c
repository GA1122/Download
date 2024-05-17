GLenum WebGL2RenderingContextBase::clientWaitSync(WebGLSync* sync,
                                                  GLbitfield flags,
                                                  GLuint64 timeout) {
  if (isContextLost() || !ValidateWebGLObject("clientWaitSync", sync))
    return GL_WAIT_FAILED;

  if (timeout > kMaxClientWaitTimeout) {
    SynthesizeGLError(GL_INVALID_OPERATION, "clientWaitSync",
                      "timeout > MAX_CLIENT_WAIT_TIMEOUT_WEBGL");
    return GL_WAIT_FAILED;
  }

  if (!(flags == 0 || flags == GL_SYNC_FLUSH_COMMANDS_BIT)) {
    SynthesizeGLError(GL_INVALID_VALUE, "clientWaitSync", "invalid flags");
    return GL_WAIT_FAILED;
  }

  if (sync->IsSignaled()) {
    return GL_ALREADY_SIGNALED;
  }

  sync->UpdateCache(ContextGL());

  if (sync->IsSignaled()) {
    return GL_CONDITION_SATISFIED;
  }

  return GL_TIMEOUT_EXPIRED;
}
