void GLES2Implementation::DrawArraysInstancedANGLE(GLenum mode,
                                                   GLint first,
                                                   GLsizei count,
                                                   GLsizei primcount) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glDrawArraysInstancedANGLE("
                     << GLES2Util::GetStringDrawMode(mode) << ", " << first
                     << ", " << count << ", " << primcount << ")");
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawArraysInstancedANGLE", "count < 0");
    return;
  }
  if (primcount < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawArraysInstancedANGLE", "primcount < 0");
    return;
  }
  if (primcount == 0) {
    return;
  }
  bool simulated = false;
  if (vertex_array_object_manager_->SupportsClientSideBuffers()) {
    GLsizei num_elements;
    if (!base::CheckAdd(first, count).AssignIfValid(&num_elements)) {
      SetGLError(GL_INVALID_VALUE, "glDrawArraysInstancedANGLE",
                 "first+count overflow");
      return;
    }
    if (!vertex_array_object_manager_->SetupSimulatedClientSideBuffers(
            "glDrawArraysInstancedANGLE", this, helper_, num_elements,
            primcount, &simulated)) {
      return;
    }
  }
  helper_->DrawArraysInstancedANGLE(mode, first, count, primcount);
  RestoreArrayBuffer(simulated);
  CheckGLError();
}
