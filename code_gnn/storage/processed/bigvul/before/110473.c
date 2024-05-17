error::Error GLES2DecoderImpl::DoDrawArrays(
    const char* function_name,
    bool instanced,
    GLenum mode,
    GLint first,
    GLsizei count,
    GLsizei primcount) {
  if (ShouldDeferDraws())
    return error::kDeferCommandUntilLater;
  if (!validators_->draw_mode.IsValid(mode)) {
    SetGLErrorInvalidEnum(function_name, mode, "mode");
    return error::kNoError;
  }
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, function_name, "count < 0");
    return error::kNoError;
  }
  if (primcount < 0) {
    SetGLError(GL_INVALID_VALUE, function_name, "primcount < 0");
    return error::kNoError;
  }
  if (!CheckBoundFramebuffersValid(function_name)) {
    return error::kNoError;
  }
  if (first < 0) {
    SetGLError(GL_INVALID_VALUE, function_name, "first < 0");
    return error::kNoError;
  }

  if (count == 0 || (instanced && primcount == 0)) {
    RenderWarning("Render count or primcount is 0.");
    return error::kNoError;
  }

  GLuint max_vertex_accessed = first + count - 1;
  if (IsDrawValid(function_name, max_vertex_accessed, primcount)) {
    if (!ClearUnclearedTextures()) {
      SetGLError(GL_INVALID_VALUE, function_name, "out of memory");
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
      bool textures_set = SetBlackTextureForNonRenderableTextures();
      ApplyDirtyState();
      if (!instanced) {
        glDrawArrays(mode, first, count);
      } else {
        glDrawArraysInstancedANGLE(mode, first, count, primcount);
      }
      ProcessPendingQueries();
      if (textures_set) {
        RestoreStateForNonRenderableTextures();
      }
      if (simulated_fixed_attribs) {
        RestoreStateForSimulatedFixedAttribs();
      }
    }
    if (simulated_attrib_0) {
      RestoreStateForAttrib(0);
    }
    if (WasContextLost()) {
      LOG(ERROR) << "  GLES2DecoderImpl: Context lost during DrawArrays.";
      return error::kLostContext;
    }
  }
  return error::kNoError;
}
