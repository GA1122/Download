error::Error GLES2DecoderImpl::HandlePathParameterfCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glPathParameterfCHROMIUM";
  const volatile gles2::cmds::PathParameterfCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::PathParameterfCHROMIUM*>(
          cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;

  GLuint service_id = 0;
  if (!path_manager()->GetPath(static_cast<GLuint>(c.path), &service_id)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                       "invalid path name");
    return error::kNoError;
  }

  GLenum pname = static_cast<GLenum>(c.pname);
  GLfloat value = static_cast<GLfloat>(c.value);
  bool hasValueError = false;

  switch (pname) {
    case GL_PATH_STROKE_WIDTH_CHROMIUM:
    case GL_PATH_MITER_LIMIT_CHROMIUM:
      hasValueError = std::isnan(value) || !std::isfinite(value) || value < 0;
      break;
    case GL_PATH_STROKE_BOUND_CHROMIUM:
      value = std::max(std::min(1.0f, value), 0.0f);
      break;
    case GL_PATH_END_CAPS_CHROMIUM:
      hasValueError = !validators_->path_parameter_cap_values.IsValid(
          static_cast<GLint>(value));
      break;
    case GL_PATH_JOIN_STYLE_CHROMIUM:
      hasValueError = !validators_->path_parameter_join_values.IsValid(
          static_cast<GLint>(value));
      break;
    default:
      DCHECK(!validators_->path_parameter.IsValid(pname));
      LOCAL_SET_GL_ERROR_INVALID_ENUM(kFunctionName, pname, "pname");
      return error::kNoError;
  }
  DCHECK(validators_->path_parameter.IsValid(pname));

  if (hasValueError) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "value not correct");
    return error::kNoError;
  }

  api()->glPathParameterfNVFn(service_id, pname, value);
  return error::kNoError;
}
