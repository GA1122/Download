void WebGL2RenderingContextBase::waitSync(WebGLSync* sync,
                                          GLbitfield flags,
                                          GLint64 timeout) {
  if (isContextLost() || !ValidateWebGLObject("waitSync", sync))
    return;

  if (flags) {
    SynthesizeGLError(GL_INVALID_VALUE, "waitSync", "invalid flags");
    return;
  }

  if (timeout != -1) {
    SynthesizeGLError(GL_INVALID_VALUE, "waitSync", "invalid timeout");
    return;
  }

}
