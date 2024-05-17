void GLES2DecoderImpl::DoGetBooleanv(GLenum pname, GLboolean* params) {
  DCHECK(params);
  GLsizei num_written = 0;
  if (GetNumValuesReturnedForGLGet(pname, &num_written)) {
    scoped_ptr<GLint[]> values(new GLint[num_written]);
    if (!state_.GetStateAsGLint(pname, values.get(), &num_written)) {
      GetHelper(pname, values.get(), &num_written);
    }
    for (GLsizei ii = 0; ii < num_written; ++ii) {
      params[ii] = static_cast<GLboolean>(values[ii]);
    }
  } else {
    pname = AdjustGetPname(pname);
    glGetBooleanv(pname, params);
  }
}
