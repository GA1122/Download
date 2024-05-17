error::Error GLES2DecoderImpl::HandleCoverFillPathCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glCoverFillPathCHROMIUM";
  const volatile gles2::cmds::CoverFillPathCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::CoverFillPathCHROMIUM*>(
          cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;

  PathCommandValidatorContext v(this, kFunctionName);
  GLenum cover_mode = GL_BOUNDING_BOX_CHROMIUM;
  if (!v.GetCoverMode(c, &cover_mode))
    return v.error();

  GLuint service_id = 0;
  if (!path_manager()->GetPath(static_cast<GLuint>(c.path), &service_id))
    return error::kNoError;
  if (!CheckBoundDrawFramebufferValid(kFunctionName))
    return error::kNoError;
  ApplyDirtyState();
  api()->glCoverFillPathNVFn(service_id, cover_mode);
  return error::kNoError;
}