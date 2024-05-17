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

  return ContextGL()->ClientWaitSync(SyncObjectOrZero(sync), flags, timeout);
}
