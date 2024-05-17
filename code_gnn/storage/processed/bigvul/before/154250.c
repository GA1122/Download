error::Error GLES2DecoderImpl::HandleGetUniformiv(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GetUniformiv& c =
      *static_cast<const volatile gles2::cmds::GetUniformiv*>(cmd_data);
  GLuint program = c.program;
  GLint fake_location = c.location;
  GLuint service_id;
  GLenum result_type;
  GLsizei result_size;
  GLint real_location = -1;
  Error error;
  cmds::GetUniformiv::Result* result;
  if (GetUniformSetup<GLint>(program, fake_location, c.params_shm_id,
                             c.params_shm_offset, &error, &real_location,
                             &service_id, &result, &result_type,
                             &result_size)) {
    api()->glGetUniformivFn(service_id, real_location, result->GetData());
  }
  return error;
}
