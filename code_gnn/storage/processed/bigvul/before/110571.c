error::Error GLES2DecoderImpl::HandleBindUniformLocationCHROMIUMImmediate(
    uint32 immediate_data_size,
    const gles2::BindUniformLocationCHROMIUMImmediate& c) {
  GLuint program = static_cast<GLuint>(c.program);
  GLint location = static_cast<GLint>(c.location);
  uint32 name_size = c.data_size;
  const char* name = GetImmediateDataAs<const char*>(
      c, name_size, immediate_data_size);
  if (name == NULL) {
    return error::kOutOfBounds;
  }
  String name_str(name, name_size);
  DoBindUniformLocationCHROMIUM(program, location, name_str.c_str());
  return error::kNoError;
}
