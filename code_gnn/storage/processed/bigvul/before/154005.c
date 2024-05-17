void GLES2DecoderImpl::DoGetBooleanv(GLenum pname,
                                     GLboolean* params,
                                     GLsizei params_size) {
  DCHECK(params);
  std::unique_ptr<GLint[]> values(new GLint[params_size]);
  memset(values.get(), 0, params_size * sizeof(GLint));
  DoGetIntegerv(pname, values.get(), params_size);
  for (GLsizei ii = 0; ii < params_size; ++ii) {
    params[ii] = static_cast<GLboolean>(values[ii]);
  }
}
