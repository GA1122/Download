error::Error GLES2DecoderImpl::HandleStencilStrokePathInstancedCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  static const char kFunctionName[] = "glStencilStrokePathInstancedCHROMIUM";
  const volatile gles2::cmds::StencilStrokePathInstancedCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::StencilStrokePathInstancedCHROMIUM*>(
          cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;

  PathCommandValidatorContext v(this, kFunctionName);
  GLuint num_paths = 0;
  GLenum path_name_type = GL_NONE;
  GLenum transform_type = GL_NONE;
  if (!v.GetPathCountAndType(c, &num_paths, &path_name_type) ||
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

  GLint reference = static_cast<GLint>(c.reference);
  GLuint mask = static_cast<GLuint>(c.mask);
  if (!CheckBoundDrawFramebufferValid(kFunctionName))
    return error::kNoError;
  ApplyDirtyState();
  api()->glStencilStrokePathInstancedNVFn(num_paths, GL_UNSIGNED_INT,
                                          paths.get(), 0, reference, mask,
                                          transform_type, transforms);
  return error::kNoError;
}
