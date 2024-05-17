error::Error GLES2DecoderPassthroughImpl::DoGetUniformBlocksCHROMIUM(
    GLuint program,
    std::vector<uint8_t>* data) {
  GLuint service_program = 0;
  if (!resources_->program_id_map.GetServiceID(program, &service_program)) {
    return error::kNoError;
  }

  GLint num_uniform_blocks = 0;
  api()->glGetProgramivFn(service_program, GL_ACTIVE_UNIFORM_BLOCKS,
                          &num_uniform_blocks);

  const base::CheckedNumeric<size_t> buffer_header_size(
      sizeof(UniformBlocksHeader));
  const base::CheckedNumeric<size_t> buffer_block_size(
      sizeof(UniformBlockInfo));
  data->resize((buffer_header_size + (num_uniform_blocks * buffer_block_size))
                   .ValueOrDie(),
               0);

  UniformBlocksHeader header;
  header.num_uniform_blocks = num_uniform_blocks;
  InsertValueIntoBuffer(data, header, 0);

  GLint active_uniform_block_max_length = 0;
  api()->glGetProgramivFn(service_program,
                          GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,
                          &active_uniform_block_max_length);

  std::vector<char> uniform_block_name_buf(active_uniform_block_max_length, 0);
  for (GLint uniform_block_index = 0; uniform_block_index < num_uniform_blocks;
       uniform_block_index++) {
    UniformBlockInfo block_info;

    GLint uniform_block_binding = 0;
    api()->glGetActiveUniformBlockivFn(service_program, uniform_block_index,
                                       GL_UNIFORM_BLOCK_BINDING,
                                       &uniform_block_binding);
    block_info.binding = uniform_block_binding;

    GLint uniform_block_data_size = 0;
    api()->glGetActiveUniformBlockivFn(service_program, uniform_block_index,
                                       GL_UNIFORM_BLOCK_DATA_SIZE,
                                       &uniform_block_data_size);
    block_info.data_size = uniform_block_data_size;

    GLint uniform_block_name_length = 0;
    api()->glGetActiveUniformBlockNameFn(
        service_program, uniform_block_index, active_uniform_block_max_length,
        &uniform_block_name_length, uniform_block_name_buf.data());

    DCHECK(uniform_block_name_length + 1 <= active_uniform_block_max_length);
    block_info.name_offset = data->size();
    block_info.name_length = uniform_block_name_length + 1;
    AppendStringToBuffer(data, uniform_block_name_buf.data(),
                         uniform_block_name_length + 1);

    GLint uniform_block_active_uniforms = 0;
    api()->glGetActiveUniformBlockivFn(service_program, uniform_block_index,
                                       GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                                       &uniform_block_active_uniforms);
    block_info.active_uniforms = uniform_block_active_uniforms;

    std::vector<GLint> uniform_block_indices_buf(uniform_block_active_uniforms,
                                                 0);
    api()->glGetActiveUniformBlockivFn(service_program, uniform_block_index,
                                       GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                                       uniform_block_indices_buf.data());
    block_info.active_uniform_offset = data->size();
    for (GLint uniform_block_uniform_index_index = 0;
         uniform_block_uniform_index_index < uniform_block_active_uniforms;
         uniform_block_uniform_index_index++) {
      AppendValueToBuffer(
          data,
          static_cast<uint32_t>(
              uniform_block_indices_buf[uniform_block_uniform_index_index]));
    }

    GLint uniform_block_referenced_by_vertex_shader = 0;
    api()->glGetActiveUniformBlockivFn(
        service_program, uniform_block_index,
        GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER,
        &uniform_block_referenced_by_vertex_shader);
    block_info.referenced_by_vertex_shader =
        uniform_block_referenced_by_vertex_shader;

    GLint uniform_block_referenced_by_fragment_shader = 0;
    api()->glGetActiveUniformBlockivFn(
        service_program, uniform_block_index,
        GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER,
        &uniform_block_referenced_by_fragment_shader);
    block_info.referenced_by_fragment_shader =
        uniform_block_referenced_by_fragment_shader;

    InsertValueIntoBuffer(
        data, block_info,
        (buffer_header_size + (buffer_block_size * uniform_block_index))
            .ValueOrDie());
  }

  return error::kNoError;
}
