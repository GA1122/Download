void WebGLRenderingContextBase::drawElements(GLenum mode,
                                             GLsizei count,
                                             GLenum type,
                                             long long offset) {
  if (!ValidateDrawElements("drawElements", type, offset))
    return;

  if (!bound_vertex_array_object_->IsAllEnabledAttribBufferBound()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "drawElements",
                      "no buffer is bound to enabled attribute");
    return;
  }

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.get());
  ClearIfComposited();
  ContextGL()->DrawElements(
      mode, count, type,
      reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
  MarkContextChanged(kCanvasChanged);
}
