void GLES2DecoderImpl::DoGetFloatv(GLenum pname, GLfloat* params) {
  DCHECK(params);
  GLsizei num_written = 0;
  if (!state_.GetStateAsGLfloat(pname, params, &num_written)) {
    if (GetHelper(pname, NULL, &num_written)) {
      scoped_ptr<GLint[]> values(new GLint[num_written]);
      GetHelper(pname, values.get(), &num_written);
      for (GLsizei ii = 0; ii < num_written; ++ii) {
        params[ii] = static_cast<GLfloat>(values[ii]);
      }
    } else {
      pname = AdjustGetPname(pname);
      glGetFloatv(pname, params);
    }
  }
}