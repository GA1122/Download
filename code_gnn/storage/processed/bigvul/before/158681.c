error::Error GLES2DecoderImpl::DoDrawArrays(
    const char* function_name,
    bool instanced,
    GLenum mode,
    GLint first,
    GLsizei count,
    GLsizei primcount) {
  error::Error error = WillAccessBoundFramebufferForDraw();
  if (error != error::kNoError)
    return error;
  if (!validators_->draw_mode.IsValid(mode)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(function_name, mode, "mode");
    return error::kNoError;
  }
  if (count < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "count < 0");
    return error::kNoError;
  }
  if (primcount < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "primcount < 0");
    return error::kNoError;
  }
  if (!CheckBoundDrawFramebufferValid(function_name)) {
    return error::kNoError;
  }
  if (first < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "first < 0");
    return error::kNoError;
  }

  if (feature_info_->IsWebGL2OrES3Context()) {
    if (!AttribsTypeMatch()) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                         "vertexAttrib function must match shader attrib type");
      return error::kNoError;
    }

    DCHECK(state_.bound_transform_feedback.get());
    if (state_.bound_transform_feedback->active() &&
        !state_.bound_transform_feedback->paused()) {
      if (mode != state_.bound_transform_feedback->primitive_mode()) {
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
            "mode differs from active transformfeedback's primitiveMode");
        return error::kNoError;
      }
      GLsizei vertices = 0;
      bool valid = state_.bound_transform_feedback->GetVerticesNeededForDraw(
          mode, count, primcount, &vertices);
      if (!valid) {
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                           "integer overflow calculating number of vertices "
                           "for transform feedback");
        return error::kNoError;
      }
      if (!buffer_manager()->RequestBuffersAccess(
              state_.GetErrorState(), state_.bound_transform_feedback.get(),
              state_.current_program->GetTransformFeedbackVaryingSizes(),
              vertices, function_name, "transformfeedback buffers")) {
        return error::kNoError;
      }
    }

    if (!ValidateUniformBlockBackings(function_name)) {
      return error::kNoError;
    }
  }

  if (count == 0 || primcount == 0) {
    LOCAL_RENDER_WARNING("Render count or primcount is 0.");
    return error::kNoError;
  }

  base::CheckedNumeric<GLuint> checked_max_vertex = first;
  checked_max_vertex += count - 1;
  if (!checked_max_vertex.IsValid()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                       "first + count overflow");
    return error::kNoError;
  }
  GLuint max_vertex_accessed = checked_max_vertex.ValueOrDefault(0);
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
      if (!ValidateAndAdjustDrawBuffers(function_name)) {
        return error::kNoError;
      }
      if (!instanced) {
        api()->glDrawArraysFn(mode, first, count);
      } else {
        api()->glDrawArraysInstancedANGLEFn(mode, first, count, primcount);
      }
      if (state_.bound_transform_feedback.get()) {
        state_.bound_transform_feedback->OnVerticesDrawn(mode, count,
                                                         primcount);
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
