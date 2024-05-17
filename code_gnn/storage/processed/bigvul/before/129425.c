error::Error GLES2DecoderImpl::HandleGetUniformiv(
    uint32 immediate_data_size, const cmds::GetUniformiv& c) {
  GLuint program = c.program;
  GLint fake_location = c.location;
  GLuint service_id;
  GLenum result_type;
  GLint real_location = -1;
  Error error;
  void* result;
  if (GetUniformSetup(
      program, fake_location, c.params_shm_id, c.params_shm_offset,
      &error, &real_location, &service_id, &result, &result_type)) {
    glGetUniformiv(
        service_id, real_location,
        static_cast<cmds::GetUniformiv::Result*>(result)->GetData());
  }
  return error;
}
