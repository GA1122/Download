error::Error GLES2DecoderImpl::HandleGetShaderPrecisionFormat(
    uint32 immediate_data_size, const cmds::GetShaderPrecisionFormat& c) {
  GLenum shader_type = static_cast<GLenum>(c.shadertype);
  GLenum precision_type = static_cast<GLenum>(c.precisiontype);
  typedef cmds::GetShaderPrecisionFormat::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (result->success != 0) {
    return error::kInvalidArguments;
  }
  if (!validators_->shader_type.IsValid(shader_type)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(
        "glGetShaderPrecisionFormat", shader_type, "shader_type");
    return error::kNoError;
  }
  if (!validators_->shader_precision.IsValid(precision_type)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(
        "glGetShaderPrecisionFormat", precision_type, "precision_type");
    return error::kNoError;
  }

  result->success = 1;   

  GLint range[2] = { 0, 0 };
  GLint precision = 0;
  GetShaderPrecisionFormatImpl(shader_type, precision_type, range, &precision);

  result->min_range = range[0];
  result->max_range = range[1];
  result->precision = precision;

  return error::kNoError;
}
