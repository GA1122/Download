error::Error GLES2DecoderPassthroughImpl::DoGetActiveUniformsiv(
    GLuint program,
    GLsizei count,
    const GLuint* indices,
    GLenum pname,
    GLint* params) {
  api()->glGetActiveUniformsivFn(GetProgramServiceID(program, resources_),
                                 count, indices, pname, params);
  return error::kNoError;
}
