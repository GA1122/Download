error::Error GLES2DecoderImpl::HandleGetUniformfv(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GetUniformfv& c =
      *static_cast<const volatile gles2::cmds::GetUniformfv*>(cmd_data);
  GLuint program = c.program;
  GLint fake_location = c.location;
  GLuint service_id;
  GLint real_location = -1;
  Error error;
  cmds::GetUniformfv::Result* result;
  GLenum result_type;
  GLsizei result_size;
  if (GetUniformSetup<GLfloat>(program, fake_location, c.params_shm_id,
                               c.params_shm_offset, &error, &real_location,
                               &service_id, &result, &result_type,
                               &result_size)) {
    if (result_type == GL_BOOL || result_type == GL_BOOL_VEC2 ||
        result_type == GL_BOOL_VEC3 || result_type == GL_BOOL_VEC4) {
      GLsizei num_values = result_size / sizeof(GLfloat);
      std::unique_ptr<GLint[]> temp(new GLint[num_values]);
      api()->glGetUniformivFn(service_id, real_location, temp.get());
      GLfloat* dst = result->GetData();
      for (GLsizei ii = 0; ii < num_values; ++ii) {
        dst[ii] = (temp[ii] != 0);
      }
    } else {
      api()->glGetUniformfvFn(service_id, real_location, result->GetData());
    }
  }
  return error;
}
