void GLES2DecoderImpl::DoGetInteger64v(GLenum pname,
                                       GLint64* params,
                                       GLsizei params_size) {
  DCHECK(params);
  if (feature_info_->IsWebGL2OrES3Context()) {
    switch (pname) {
      case GL_MAX_ELEMENT_INDEX: {
        DCHECK_EQ(params_size, 1);
        if (gl_version_info().IsAtLeastGLES(3, 0) ||
            gl_version_info().IsAtLeastGL(4, 3)) {
          api()->glGetInteger64vFn(GL_MAX_ELEMENT_INDEX, params);
        } else {
          if (params) {
            *params = std::numeric_limits<unsigned int>::max();
          }
        }
        return;
      }
    }
  }

  std::unique_ptr<GLint[]> values(new GLint[params_size]);
  memset(values.get(), 0, params_size * sizeof(GLint));
  DoGetIntegerv(pname, values.get(), params_size);
  for (GLsizei ii = 0; ii < params_size; ++ii) {
    params[ii] = static_cast<GLint64>(values[ii]);
  }
}
