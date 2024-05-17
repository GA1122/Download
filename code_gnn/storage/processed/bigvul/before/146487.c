void WebGLRenderingContextBase::drawArrays(GLenum mode,
                                           GLint first,
                                           GLsizei count) {
  if (!ValidateDrawArrays("drawArrays"))
    return;

  if (!bound_vertex_array_object_->IsAllEnabledAttribBufferBound()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "drawArrays",
                      "no buffer is bound to enabled attribute");
    return;
  }

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.Get());
  ClearIfComposited();
  ContextGL()->DrawArrays(mode, first, count);
  MarkContextChanged(kCanvasChanged);
}
