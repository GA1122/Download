void GLES2Implementation::DeletePathsCHROMIUM(GLuint first_client_id,
                                              GLsizei range) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glDeletePathsCHROMIUM("
                     << first_client_id << ", " << range << ")");
  static const char kFunctionName[] = "glDeletePathsCHROMIUM";

  if (range < 0) {
    SetGLError(GL_INVALID_VALUE, kFunctionName, "range < 0");
    return;
  }
  if (!base::IsValueInRangeForNumericType<int32_t>(range)) {
    SetGLError(GL_INVALID_OPERATION, kFunctionName, "range more than 32-bit");
    return;
  }
  if (range == 0)
    return;

  GLuint last_client_id;
  if (!base::CheckAdd(first_client_id, range - 1)
           .AssignIfValid(&last_client_id)) {
    SetGLError(GL_INVALID_OPERATION, kFunctionName, "overflow");
    return;
  }

  GetRangeIdHandler(id_namespaces::kPaths)
      ->FreeIdRange(this, first_client_id, range,
                    &GLES2Implementation::DeletePathsCHROMIUMStub);
  CheckGLError();
}
