bool GLES2DecoderImpl::ValidateAndAdjustDrawBuffers(const char* func_name) {
  if (!SupportsDrawBuffers()) {
    return true;
  }
  Framebuffer* framebuffer = framebuffer_state_.bound_draw_framebuffer.get();
  if (!state_.current_program.get() || !framebuffer) {
    return true;
  }
  if (!state_.color_mask_red && !state_.color_mask_green &&
      !state_.color_mask_blue && !state_.color_mask_alpha) {
    return true;
  }
  uint32_t fragment_output_type_mask =
      state_.current_program->fragment_output_type_mask();
  uint32_t fragment_output_written_mask =
      state_.current_program->fragment_output_written_mask();
  if (!framebuffer->ValidateAndAdjustDrawBuffers(
      fragment_output_type_mask, fragment_output_written_mask)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
          "buffer format and fragment output variable type incompatible");
      return false;
  }
  return true;
}
