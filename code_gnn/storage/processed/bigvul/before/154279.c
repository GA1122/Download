error::Error GLES2DecoderImpl::HandleStencilFillPathInstancedCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glStencilFillPathInstancedCHROMIUM";
  const volatile gles2::cmds::StencilFillPathInstancedCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::StencilFillPathInstancedCHROMIUM*>(
          cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;

  PathCommandValidatorContext v(this, kFunctionName);
  GLuint num_paths = 0;
  GLenum path_name_type = GL_NONE;
  GLenum fill_mode = GL_COUNT_UP_CHROMIUM;
  GLuint mask = 0;
  GLenum transform_type = GL_NONE;
  if (!v.GetPathCountAndType(c, &num_paths, &path_name_type) ||
      !v.GetFillModeAndMask(c, &fill_mode, &mask) ||
      !v.GetTransformType(c, &transform_type))
    return v.error();

  if (num_paths == 0)
    return error::kNoError;

  std::unique_ptr<GLuint[]> paths;
  if (!v.GetPathNameData(c, num_paths, path_name_type, &paths))
    return v.error();

  const GLfloat* transforms = nullptr;
  if (!v.GetTransforms(c, num_paths, transform_type, &transforms))
    return v.error();

  if (!CheckBoundDrawFramebufferValid(kFunctionName))
    return error::kNoError;
  ApplyDirtyState();
  api()->glStencilFillPathInstancedNVFn(num_paths, GL_UNSIGNED_INT, paths.get(),
                                        0, fill_mode, mask, transform_type,
                                        transforms);
  return error::kNoError;
}
