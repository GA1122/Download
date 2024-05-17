void GLES2DecoderImpl::DoGetFloatv(GLenum pname,
                                   GLfloat* params,
                                   GLsizei params_size) {
  DCHECK(params);
  GLsizei num_written = 0;
  if (state_.GetStateAsGLfloat(pname, params, &num_written)) {
    DCHECK_EQ(num_written, params_size);
    return;
  }

  switch (pname) {
    case GL_ALIASED_POINT_SIZE_RANGE:
    case GL_ALIASED_LINE_WIDTH_RANGE:
    case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:
      DCHECK_EQ(params_size, util_.GLGetNumValuesReturned(pname));
      pname = AdjustGetPname(pname);
      api()->glGetFloatvFn(pname, params);
      return;
  }

  std::unique_ptr<GLint[]> values(new GLint[params_size]);
  memset(values.get(), 0, params_size * sizeof(GLint));
  DoGetIntegerv(pname, values.get(), params_size);
  for (GLsizei ii = 0; ii < params_size; ++ii) {
    params[ii] = static_cast<GLfloat>(values[ii]);
  }
}
