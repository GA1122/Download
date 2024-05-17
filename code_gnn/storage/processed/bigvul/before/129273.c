error::Error GLES2DecoderImpl::DoDrawElements(
    const char* function_name,
    bool instanced,
    GLenum mode,
    GLsizei count,
    GLenum type,
    int32 offset,
    GLsizei primcount) {
  error::Error error = WillAccessBoundFramebufferForDraw();
  if (error != error::kNoError)
    return error;
  if (!state_.vertex_attrib_manager->element_array_buffer()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "No element array buffer bound");
    return error::kNoError;
  }

  if (count < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "count < 0");
    return error::kNoError;
  }
  if (offset < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "offset < 0");
    return error::kNoError;
  }
  if (!validators_->draw_mode.IsValid(mode)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(function_name, mode, "mode");
    return error::kNoError;
  }
  if (!validators_->index_type.IsValid(type)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(function_name, type, "type");
    return error::kNoError;
  }
  if (primcount < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "primcount < 0");
    return error::kNoError;
  }

  if (!CheckBoundFramebuffersValid(function_name)) {
    return error::kNoError;
  }

  if (count == 0 || (instanced && primcount == 0)) {
    return error::kNoError;
  }

  GLuint max_vertex_accessed;
  Buffer* element_array_buffer =
      state_.vertex_attrib_manager->element_array_buffer();

  if (!element_array_buffer->GetMaxValueForRange(
      offset, count, type, &max_vertex_accessed)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "range out of bounds for buffer");
    return error::kNoError;
  }

  if (IsDrawValid(function_name, max_vertex_accessed, primcount)) {
    if (!ClearUnclearedTextures()) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "out of memory");
      return error::kNoError;
    }
    bool simulated_attrib_0 = false;
    if (!SimulateAttrib0(
        function_name, max_vertex_accessed, &simulated_attrib_0)) {
      return error::kNoError;
    }
    bool simulated_fixed_attribs = false;
    if (SimulateFixedAttribs(
        function_name, max_vertex_accessed, &simulated_fixed_attribs,
        primcount)) {
      bool textures_set = !PrepareTexturesForRender();
      ApplyDirtyState();
      const GLvoid* indices = reinterpret_cast<const GLvoid*>(offset);
      bool used_client_side_array = false;
      if (element_array_buffer->IsClientSideArray()) {
        used_client_side_array = true;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        indices = element_array_buffer->GetRange(offset, 0);
      }

      ScopedRenderTo do_render(framebuffer_state_.bound_draw_framebuffer.get());
      if (!instanced) {
        glDrawElements(mode, count, type, indices);
      } else {
        glDrawElementsInstancedANGLE(mode, count, type, indices, primcount);
      }

      if (used_client_side_array) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     element_array_buffer->service_id());
      }

      ProcessPendingQueries();
      if (textures_set) {
        RestoreStateForTextures();
      }
      if (simulated_fixed_attribs) {
        RestoreStateForSimulatedFixedAttribs();
      }
    }
    if (simulated_attrib_0) {
      RestoreStateForAttrib(0, false);
    }
  }
  return error::kNoError;
}
