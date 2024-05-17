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
  GLsizei num_values = 0;
  std::vector<GLint> samples;
  if (gl_version_info().IsLowerThanGL(4, 2)) {
    if (!GLES2Util::IsIntegerFormat(format)) {
      GLint max_samples = renderbuffer_manager()->max_samples();
      while (max_samples > 0) {
        samples.push_back(max_samples);
        --max_samples;
      }
    }
    switch (pname) {
      case GL_NUM_SAMPLE_COUNTS:
        num_values = 1;
        break;
      case GL_SAMPLES:
        num_values = static_cast<GLsizei>(samples.size());
        break;
      default:
        NOTREACHED();
        break;
    }
  } else {
    switch (pname) {
      case GL_NUM_SAMPLE_COUNTS:
        num_values = 1;
        break;
      case GL_SAMPLES:
        {
          GLint value = 0;
          api()->glGetInternalformativFn(target, format, GL_NUM_SAMPLE_COUNTS,
                                         1, &value);
          num_values = static_cast<GLsizei>(value);
        }
        break;
      default:
        NOTREACHED();
        break;
    }
  }
  Result* result = GetSharedMemoryAs<Result*>(
      c.params_shm_id, c.params_shm_offset, Result::ComputeSize(num_values));
  GLint* params = result ? result->GetData() : nullptr;
  if (params == nullptr) {
    return error::kOutOfBounds;
  }
  if (result->size != 0) {
    return error::kInvalidArguments;
  }
  if (gl_version_info().IsLowerThanGL(4, 2)) {
    switch (pname) {
      case GL_NUM_SAMPLE_COUNTS:
        params[0] = static_cast<GLint>(samples.size());
        break;
      case GL_SAMPLES:
        for (size_t ii = 0; ii < samples.size(); ++ii) {
          params[ii] = samples[ii];
        }
        break;
      default:
        NOTREACHED();
        break;
    }
  } else {
    api()->glGetInternalformativFn(target, format, pname, num_values, params);
  }
  result->SetNumResults(num_values);
  return error::kNoError;
}
