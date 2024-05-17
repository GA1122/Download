void GLES2DecoderImpl::DoGetIntegerv(GLenum pname, GLint* params) {
  DCHECK(params);
  GLsizei num_written;
  if (!state_.GetStateAsGLint(pname, params, &num_written) &&
      !GetHelper(pname, params, &num_written)) {
    pname = AdjustGetPname(pname);
    glGetIntegerv(pname, params);
  }
}
