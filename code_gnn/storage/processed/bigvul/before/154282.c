error::Error GLES2DecoderImpl::HandleStencilThenCoverFillPathCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glStencilThenCoverFillPathCHROMIUM";
  const volatile gles2::cmds::StencilThenCoverFillPathCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::StencilThenCoverFillPathCHROMIUM*>(
          cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;

  PathCommandValidatorContext v(this, kFunctionName);
  GLenum fill_mode = GL_COUNT_UP_CHROMIUM;
  GLuint mask = 0;
  GLenum cover_mode = GL_BOUNDING_BOX_CHROMIUM;
  if (!v.GetFillModeAndMask(c, &fill_mode, &mask) ||
      !v.GetCoverMode(c, &cover_mode))
    return v.error();

  GLuint service_id = 0;
  if (!path_manager()->GetPath(static_cast<GLuint>(c.path), &service_id))
    return error::kNoError;

  if (!CheckBoundDrawFramebufferValid(kFunctionName))
    return error::kNoError;
  ApplyDirtyState();
  api()->glStencilThenCoverFillPathNVFn(service_id, fill_mode, mask,
                                        cover_mode);
  return error::kNoError;
}
