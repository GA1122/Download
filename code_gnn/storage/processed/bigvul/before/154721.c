error::Error GLES2DecoderPassthroughImpl::DoGetUniformsES3CHROMIUM(
    GLuint program,
    std::vector<uint8_t>* data) {
  GLuint service_program = 0;
  if (!resources_->program_id_map.GetServiceID(program, &service_program)) {
    return error::kNoError;
  }

  GLint num_uniforms = 0;
  api()->glGetProgramivFn(service_program, GL_ACTIVE_UNIFORMS, &num_uniforms);

  UniformsES3Header header;
  header.num_uniforms = num_uniforms;
  AppendValueToBuffer(data, header);

  for (GLuint uniform_index = 0;
       uniform_index < static_cast<GLuint>(num_uniforms); uniform_index++) {
    UniformES3Info uniform_info;

    GLint uniform_block_index = 0;
    api()->glGetActiveUniformsivFn(service_program, 1, &uniform_index,
                                   GL_UNIFORM_BLOCK_INDEX,
                                   &uniform_block_index);
    uniform_info.block_index = uniform_block_index;

    GLint uniform_offset = 0;
    api()->glGetActiveUniformsivFn(service_program, 1, &uniform_index,
                                   GL_UNIFORM_OFFSET, &uniform_offset);
    uniform_info.offset = uniform_offset;

    GLint uniform_array_stride = 0;
    api()->glGetActiveUniformsivFn(service_program, 1, &uniform_index,
                                   GL_UNIFORM_ARRAY_STRIDE,
                                   &uniform_array_stride);
    uniform_info.array_stride = uniform_array_stride;

    GLint uniform_matrix_stride = 0;
    api()->glGetActiveUniformsivFn(service_program, 1, &uniform_index,
                                   GL_UNIFORM_MATRIX_STRIDE,
                                   &uniform_matrix_stride);
    uniform_info.matrix_stride = uniform_matrix_stride;

    GLint uniform_is_row_major = 0;
    api()->glGetActiveUniformsivFn(service_program, 1, &uniform_index,
                                   GL_UNIFORM_IS_ROW_MAJOR,
                                   &uniform_is_row_major);
    uniform_info.is_row_major = uniform_is_row_major;

    AppendValueToBuffer(data, uniform_info);
  }

  return error::kNoError;
}
