error::Error GLES2DecoderPassthroughImpl::DoGetProgramInfoCHROMIUM(
    GLuint program,
    std::vector<uint8_t>* data) {
  GLuint service_program = 0;
  if (!resources_->program_id_map.GetServiceID(program, &service_program)) {
    return error::kNoError;
  }

  GLint num_attributes = 0;
  api()->glGetProgramivFn(service_program, GL_ACTIVE_ATTRIBUTES,
                          &num_attributes);

  GLint num_uniforms = 0;
  api()->glGetProgramivFn(service_program, GL_ACTIVE_UNIFORMS, &num_uniforms);

  const base::CheckedNumeric<size_t> buffer_header_size(
      sizeof(ProgramInfoHeader));
  const base::CheckedNumeric<size_t> buffer_block_size(sizeof(ProgramInput));
  const base::CheckedNumeric<size_t> attribute_block_size =
      buffer_block_size * num_attributes;
  const base::CheckedNumeric<size_t> uniform_block_size =
      buffer_block_size * num_uniforms;
  data->resize((buffer_header_size + attribute_block_size + uniform_block_size)
                   .ValueOrDie(),
               0);

  GLint link_status = 0;
  api()->glGetProgramivFn(service_program, GL_LINK_STATUS, &link_status);

  ProgramInfoHeader header;
  header.link_status = link_status;
  header.num_attribs = num_attributes;
  header.num_uniforms = num_uniforms;
  InsertValueIntoBuffer(data, header, 0);

  GLint active_attribute_max_length = 0;
  api()->glGetProgramivFn(service_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                          &active_attribute_max_length);

  std::vector<char> attrib_name_buf(active_attribute_max_length, 0);
  for (GLint attrib_index = 0; attrib_index < num_attributes; attrib_index++) {
    GLsizei length = 0;
    GLint size = 0;
    GLenum type = GL_NONE;
    api()->glGetActiveAttribFn(service_program, attrib_index,
                               attrib_name_buf.size(), &length, &size, &type,
                               attrib_name_buf.data());

    ProgramInput input;
    input.size = size;
    input.type = type;

    int32_t location =
        api()->glGetAttribLocationFn(service_program, attrib_name_buf.data());
    input.location_offset = data->size();
    AppendValueToBuffer(data, location);

    input.name_offset = data->size();
    input.name_length = length;
    AppendStringToBuffer(data, attrib_name_buf.data(), length);

    InsertValueIntoBuffer(
        data, input,
        (buffer_header_size + (buffer_block_size * attrib_index)).ValueOrDie());
  }

  GLint active_uniform_max_length = 0;
  api()->glGetProgramivFn(service_program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                          &active_uniform_max_length);

  std::vector<char> uniform_name_buf(active_uniform_max_length, 0);
  for (GLint uniform_index = 0; uniform_index < num_uniforms; uniform_index++) {
    GLsizei length = 0;
    GLint size = 0;
    GLenum type = GL_NONE;
    api()->glGetActiveUniformFn(service_program, uniform_index,
                                uniform_name_buf.size(), &length, &size, &type,
                                uniform_name_buf.data());

    ProgramInput input;
    input.size = size;
    input.type = type;

    input.location_offset = data->size();
    int32_t base_location =
        api()->glGetUniformLocationFn(service_program, uniform_name_buf.data());
    AppendValueToBuffer(data, base_location);

    GLSLArrayName parsed_service_name(uniform_name_buf.data());
    if (size > 1 || parsed_service_name.IsArrayName()) {
      for (GLint location_index = 1; location_index < size; location_index++) {
        std::string array_element_name = parsed_service_name.base_name() + "[" +
                                         base::NumberToString(location_index) +
                                         "]";
        int32_t element_location = api()->glGetUniformLocationFn(
            service_program, array_element_name.c_str());
        AppendValueToBuffer(data, element_location);
      }
    }

    input.name_offset = data->size();
    input.name_length = length;
    AppendStringToBuffer(data, uniform_name_buf.data(), length);

    InsertValueIntoBuffer(data, input,
                          (buffer_header_size + attribute_block_size +
                           (buffer_block_size * uniform_index))
                              .ValueOrDie());
  }

  return error::kNoError;
}
