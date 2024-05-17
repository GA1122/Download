error::Error GLES2DecoderPassthroughImpl::DoGetProgramResourceLocation(
    GLuint program,
    GLenum program_interface,
    const char* name,
    GLint* location) {
  *location = api()->glGetProgramResourceLocationFn(
      GetProgramServiceID(program, resources_), program_interface, name);
  return error::kNoError;
}
