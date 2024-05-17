error::Error GLES2DecoderImpl::HandleStencilStrokePathCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glStencilStrokePathCHROMIUM";
  const volatile gles2::cmds::StencilStrokePathCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::StencilStrokePathCHROMIUM*>(
          cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;

  GLuint service_id = 0;
  if (!path_manager()->GetPath(static_cast<GLuint>(c.path), &service_id)) {
    return error::kNoError;
  }
  GLint reference = static_cast<GLint>(c.reference);
  GLuint mask = static_cast<GLuint>(c.mask);
  if (!CheckBoundDrawFramebufferValid(kFunctionName))
    return error::kNoError;
  ApplyDirtyState();
  api()->glStencilStrokePathNVFn(service_id, reference, mask);
  return error::kNoError;
}
