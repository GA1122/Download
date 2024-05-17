void GLES2Implementation::DrawArrays(GLenum mode, GLint first, GLsizei count) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glDrawArrays("
                     << GLES2Util::GetStringDrawMode(mode) << ", " << first
                     << ", " << count << ")");
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawArrays", "count < 0");
    return;
  }
  bool simulated = false;
  if (vertex_array_object_manager_->SupportsClientSideBuffers()) {
    GLsizei num_elements;
    if (!base::CheckAdd(first, count).AssignIfValid(&num_elements)) {
      SetGLError(GL_INVALID_VALUE, "glDrawArrays", "first+count overflow");
      return;
    }
    if (!vertex_array_object_manager_->SetupSimulatedClientSideBuffers(
            "glDrawArrays", this, helper_, num_elements, 0, &simulated)) {
      return;
    }
  }
  helper_->DrawArrays(mode, first, count);
  RestoreArrayBuffer(simulated);
  CheckGLError();
}
