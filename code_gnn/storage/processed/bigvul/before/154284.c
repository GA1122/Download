error::Error GLES2DecoderImpl::HandleStencilThenCoverStrokePathCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glStencilThenCoverStrokePathCHROMIUM";
  const volatile gles2::cmds::StencilThenCoverStrokePathCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::StencilThenCoverStrokePathCHROMIUM*>(
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

  GLint reference = static_cast<GLint>(c.reference);
  GLuint mask = static_cast<GLuint>(c.mask);

  if (!CheckBoundDrawFramebufferValid(kFunctionName))
    return error::kNoError;
  ApplyDirtyState();
  api()->glStencilThenCoverStrokePathNVFn(service_id, reference, mask,
                                          cover_mode);
  return error::kNoError;
}
