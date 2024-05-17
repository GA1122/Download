error::Error GLES2DecoderImpl::HandleDeletePathsCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::DeletePathsCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::DeletePathsCHROMIUM*>(cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;

  PathCommandValidatorContext v(this, "glDeletePathsCHROMIUM");
  GLsizei range = 0;
  if (!v.GetPathRange(c, &range))
    return v.error();

  if (range == 0)
    return error::kNoError;

  GLuint first_client_id = c.first_client_id;

  if (!DeletePathsCHROMIUMHelper(first_client_id, range))
    return error::kInvalidArguments;

  return error::kNoError;
}
