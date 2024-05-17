void GLES2Implementation::GetShaderPrecisionFormat(GLenum shadertype,
                                                   GLenum precisiontype,
                                                   GLint* range,
                                                   GLint* precision) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetShaderPrecisionFormat("
                     << GLES2Util::GetStringShaderType(shadertype) << ", "
                     << GLES2Util::GetStringShaderPrecision(precisiontype)
                     << ", " << static_cast<const void*>(range) << ", "
                     << static_cast<const void*>(precision) << ", ");
  TRACE_EVENT0("gpu", "GLES2::GetShaderPrecisionFormat");
  typedef cmds::GetShaderPrecisionFormat::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return;
  }

  GLStaticState::ShaderPrecisionKey key(shadertype, precisiontype);
  GLStaticState::ShaderPrecisionMap::iterator i =
      static_state_.shader_precisions.find(key);
  if (i != static_state_.shader_precisions.end()) {
    *result = i->second;
  } else {
    result->success = false;
    helper_->GetShaderPrecisionFormat(shadertype, precisiontype,
                                      GetResultShmId(), result.offset());
    WaitForCmd();
    if (result->success)
      static_state_.shader_precisions[key] = *result;
  }

  if (result->success) {
    if (range) {
      range[0] = result->min_range;
      range[1] = result->max_range;
      GPU_CLIENT_LOG("  min_range: " << range[0]);
      GPU_CLIENT_LOG("  min_range: " << range[1]);
    }
    if (precision) {
      precision[0] = result->precision;
      GPU_CLIENT_LOG("  min_range: " << precision[0]);
    }
  }
  CheckGLError();
}
