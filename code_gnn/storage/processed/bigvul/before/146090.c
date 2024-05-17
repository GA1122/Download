void WebGL2RenderingContextBase::drawRangeElements(GLenum mode,
                                                   GLuint start,
                                                   GLuint end,
                                                   GLsizei count,
                                                   GLenum type,
                                                   long long offset) {
  if (!ValidateDrawElements("drawRangeElements", type, offset))
    return;

  if (!bound_vertex_array_object_->IsAllEnabledAttribBufferBound()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "drawRangeElements",
                      "no buffer is bound to enabled attribute");
    return;
  }

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.Get());
  ClearIfComposited();
  ContextGL()->DrawRangeElements(
      mode, start, end, count, type,
      reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
  MarkContextChanged(kCanvasChanged);
}
