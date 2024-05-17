error::Error GLES2DecoderImpl::DoDrawElements(const char* function_name,
                                              bool instanced,
                                              GLenum mode,
                                              GLsizei count,
                                              GLenum type,
                                              int32_t offset,
                                              GLsizei primcount) {
  error::Error error = WillAccessBoundFramebufferForDraw();
  if (error != error::kNoError)
    return error;

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
  Buffer* element_array_buffer = buffer_manager()->RequestBufferAccess(
      &state_, GL_ELEMENT_ARRAY_BUFFER, function_name);
  if (!element_array_buffer) {
    return error::kNoError;
  }

  if (!CheckBoundDrawFramebufferValid(function_name)) {
    return error::kNoError;
  }

  if (state_.bound_transform_feedback.get() &&
      state_.bound_transform_feedback->active() &&
      !state_.bound_transform_feedback->paused()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
        "transformfeedback is active and not paused");
    return error::kNoError;
  }

  if (feature_info_->IsWebGL2OrES3Context()) {
    if (!AttribsTypeMatch()) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                         "vertexAttrib function must match shader attrib type");
      return error::kNoError;
    }
    if (!ValidateUniformBlockBackings(function_name)) {
      return error::kNoError;
    }
  }

  if (count == 0 || primcount == 0) {
    return error::kNoError;
  }

  GLuint max_vertex_accessed;
  if (!element_array_buffer->GetMaxValueForRange(
          offset, count, type,
          state_.enable_flags.primitive_restart_fixed_index,
          &max_vertex_accessed)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "range out of bounds for buffer");
    return error::kNoError;
  }

  if (IsDrawValid(function_name, max_vertex_accessed, instanced, primcount)) {
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
        api()->glBindBufferFn(GL_ELEMENT_ARRAY_BUFFER, 0);
        indices = element_array_buffer->GetRange(offset, 0);
      }
      if (!ValidateAndAdjustDrawBuffers(function_name)) {
        return error::kNoError;
      }
      if (state_.enable_flags.primitive_restart_fixed_index &&
          feature_info_->feature_flags().
              emulate_primitive_restart_fixed_index) {
        api()->glEnableFn(GL_PRIMITIVE_RESTART);
        buffer_manager()->SetPrimitiveRestartFixedIndexIfNecessary(type);
      }
      if (!instanced) {
        api()->glDrawElementsFn(mode, count, type, indices);
      } else {
        api()->glDrawElementsInstancedANGLEFn(mode, count, type, indices,
                                              primcount);
      }
      if (state_.enable_flags.primitive_restart_fixed_index &&
          feature_info_->feature_flags().
              emulate_primitive_restart_fixed_index) {
        api()->glDisableFn(GL_PRIMITIVE_RESTART);
      }
      if (used_client_side_array) {
        api()->glBindBufferFn(GL_ELEMENT_ARRAY_BUFFER,
                              element_array_buffer->service_id());
      }
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
