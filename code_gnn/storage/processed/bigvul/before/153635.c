void GLES2Implementation::DrawElementsInstancedANGLE(GLenum mode,
                                                     GLsizei count,
                                                     GLenum type,
                                                     const void* indices,
                                                     GLsizei primcount) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glDrawElementsInstancedANGLE("
                     << GLES2Util::GetStringDrawMode(mode) << ", " << count
                     << ", " << GLES2Util::GetStringIndexType(type) << ", "
                     << static_cast<const void*>(indices) << ", " << primcount
                     << ")");
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawElementsInstancedANGLE",
               "count less than 0.");
    return;
  }
  if (primcount < 0) {
    SetGLError(GL_INVALID_VALUE, "glDrawElementsInstancedANGLE",
               "primcount < 0");
    return;
  }
  GLuint offset = 0;
  bool simulated = false;
  if (count > 0 && primcount > 0) {
    if (vertex_array_object_manager_->bound_element_array_buffer() != 0 &&
        !ValidateOffset("glDrawElementsInstancedANGLE",
                        reinterpret_cast<GLintptr>(indices))) {
      return;
    }
    if (!vertex_array_object_manager_->SetupSimulatedIndexAndClientSideBuffers(
            "glDrawElementsInstancedANGLE", this, helper_, count, type,
            primcount, indices, &offset, &simulated)) {
      return;
    }
  }
  helper_->DrawElementsInstancedANGLE(mode, count, type, offset, primcount);
  RestoreElementAndArrayBuffers(simulated);
  CheckGLError();
}
