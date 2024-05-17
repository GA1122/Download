GLES2DecoderPassthroughImpl::DoGetTransformFeedbackVaryingsCHROMIUM(
    GLuint program,
    std::vector<uint8_t>* data) {
  GLuint service_program = 0;
  if (!resources_->program_id_map.GetServiceID(program, &service_program)) {
    return error::kNoError;
  }

  GLint transform_feedback_buffer_mode = 0;
  api()->glGetProgramivFn(service_program, GL_TRANSFORM_FEEDBACK_BUFFER_MODE,
                          &transform_feedback_buffer_mode);

  GLint num_transform_feedback_varyings = 0;
  api()->glGetProgramivFn(service_program, GL_TRANSFORM_FEEDBACK_VARYINGS,
                          &num_transform_feedback_varyings);

  const base::CheckedNumeric<size_t> buffer_header_size(
      sizeof(TransformFeedbackVaryingsHeader));
  const base::CheckedNumeric<size_t> buffer_block_size(
      sizeof(TransformFeedbackVaryingInfo));
  data->resize((buffer_header_size +
                (num_transform_feedback_varyings * buffer_block_size))
                   .ValueOrDie(),
               0);

  TransformFeedbackVaryingsHeader header;
  header.transform_feedback_buffer_mode = transform_feedback_buffer_mode;
  header.num_transform_feedback_varyings = num_transform_feedback_varyings;
  InsertValueIntoBuffer(data, header, 0);

  GLint max_transform_feedback_varying_length = 0;
  api()->glGetProgramivFn(service_program,
                          GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,
                          &max_transform_feedback_varying_length);

  std::vector<char> transform_feedback_varying_name_buf(
      max_transform_feedback_varying_length, 0);
  for (GLint transform_feedback_varying_index = 0;
       transform_feedback_varying_index < num_transform_feedback_varyings;
       transform_feedback_varying_index++) {
    GLsizei length = 0;
    GLint size = 0;
    GLenum type = GL_NONE;
    api()->glGetTransformFeedbackVaryingFn(
        service_program, transform_feedback_varying_index,
        max_transform_feedback_varying_length, &length, &size, &type,
        transform_feedback_varying_name_buf.data());

    TransformFeedbackVaryingInfo varying_info;
    varying_info.size = size;
    varying_info.type = type;

    DCHECK(length + 1 <= max_transform_feedback_varying_length);
    varying_info.name_length = data->size();
    varying_info.name_length = length + 1;
    AppendStringToBuffer(data, transform_feedback_varying_name_buf.data(),
                         length + 1);

    InsertValueIntoBuffer(
        data, varying_info,
        (buffer_header_size +
         (buffer_block_size * transform_feedback_varying_index))
            .ValueOrDie());
  }

  return error::kNoError;
}
