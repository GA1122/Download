error::Error GLES2DecoderImpl::HandleStencilFillPathCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glStencilFillPathCHROMIUM";
  const volatile gles2::cmds::StencilFillPathCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::StencilFillPathCHROMIUM*>(
          cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;
  PathCommandValidatorContext v(this, kFunctionName);
  GLenum fill_mode = GL_COUNT_UP_CHROMIUM;
  GLuint mask = 0;
  if (!v.GetFillModeAndMask(c, &fill_mode, &mask))
    return v.error();
  GLuint service_id = 0;
  if (!path_manager()->GetPath(static_cast<GLuint>(c.path), &service_id)) {
    return error::kNoError;
  }
  if (!CheckBoundDrawFramebufferValid(kFunctionName))
    return error::kNoError;
  ApplyDirtyState();
  api()->glStencilFillPathNVFn(service_id, fill_mode, mask);
  return error::kNoError;
}
