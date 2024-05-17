error::Error GLES2DecoderImpl::HandleGetInternalformativ(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::GetInternalformativ& c =
      *static_cast<const volatile gles2::cmds::GetInternalformativ*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  GLenum format = static_cast<GLenum>(c.format);
  GLenum pname = static_cast<GLenum>(c.pname);
  if (!validators_->render_buffer_target.IsValid(target)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glGetInternalformativ", target, "target");
    return error::kNoError;
  }
  if (!validators_->render_buffer_format.IsValid(format)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glGetInternalformativ", format,
                                    "internalformat");
    return error::kNoError;
  }
  if (!validators_->internal_format_parameter.IsValid(pname)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glGetInternalformativ", pname, "pname");
    return error::kNoError;
  }

  typedef cmds::GetInternalformativ::Result Result;

  GLsizei num_sample_counts = 0;
  std::vector<GLint> sample_counts;

  GLsizei num_values = 0;
  GLint* values = nullptr;
  switch (pname) {
    case GL_NUM_SAMPLE_COUNTS:
      num_sample_counts =
          InternalFormatSampleCountsHelper(target, format, nullptr);
      num_values = 1;
      values = &num_sample_counts;
      break;
    case GL_SAMPLES:
      num_sample_counts =
          InternalFormatSampleCountsHelper(target, format, &sample_counts);
      num_values = num_sample_counts;
      values = sample_counts.data();
      break;
    default:
      NOTREACHED();
      break;
  }

  uint32_t checked_size = 0;
  if (!Result::ComputeSize(num_values).AssignIfValid(&checked_size)) {
    return error::kOutOfBounds;
  }
  Result* result = GetSharedMemoryAs<Result*>(
      c.params_shm_id, c.params_shm_offset, checked_size);

  GLint* params = result ? result->GetData() : nullptr;
  if (params == nullptr) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }

  std::copy(values, &values[num_values], params);
  result->SetNumResults(num_values);
  return error::kNoError;
}
