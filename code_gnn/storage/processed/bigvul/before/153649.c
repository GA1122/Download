GLuint GLES2Implementation::GenPathsCHROMIUM(GLsizei range) {
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGenPathsCHROMIUM(" << range
                     << ")");
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  static const char kFunctionName[] = "glGenPathsCHROMIUM";
  if (range < 0) {
    SetGLError(GL_INVALID_VALUE, kFunctionName, "range < 0");
    return 0;
  }
  if (!base::IsValueInRangeForNumericType<int32_t>(range)) {
    SetGLError(GL_INVALID_OPERATION, kFunctionName, "range more than 32-bit");
    return 0;
  }
  if (range == 0)
    return 0;

  GLuint first_client_id = 0;
  GetRangeIdHandler(id_namespaces::kPaths)
      ->MakeIdRange(this, range, &first_client_id);

  if (first_client_id == 0) {
    return 0;
  }

  helper_->GenPathsCHROMIUM(first_client_id, range);

  GPU_CLIENT_LOG_CODE_BLOCK({
    for (GLsizei i = 0; i < range; ++i) {
      GPU_CLIENT_LOG("  " << i << ": " << (first_client_id + i));
    }
  });
  CheckGLError();
  return first_client_id;
}
