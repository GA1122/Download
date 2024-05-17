error::Error GLES2DecoderImpl::HandleGenPathsCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GenPathsCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::GenPathsCHROMIUM*>(cmd_data);
  if (!features().chromium_path_rendering)
    return error::kUnknownCommand;

  PathCommandValidatorContext v(this, "glGenPathsCHROMIUM");
  GLsizei range = 0;
  if (!v.GetPathRange(c, &range))
    return v.error();

  GLuint first_client_id = static_cast<GLuint>(c.first_client_id);
  if (first_client_id == 0)
    return error::kInvalidArguments;

  if (range == 0)
    return error::kNoError;

  if (!GenPathsCHROMIUMHelper(first_client_id, range))
    return error::kInvalidArguments;

  return error::kNoError;
}
