void WebGLRenderingContextBase::DrawArraysInstancedANGLE(GLenum mode,
                                                         GLint first,
                                                         GLsizei count,
                                                         GLsizei primcount) {
  if (!ValidateDrawArrays("drawArraysInstancedANGLE"))
    return;

  if (!bound_vertex_array_object_->IsAllEnabledAttribBufferBound()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "drawArraysInstancedANGLE",
                      "no buffer is bound to enabled attribute");
    return;
  }

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.get());
  ClearIfComposited();
  ContextGL()->DrawArraysInstancedANGLE(mode, first, count, primcount);
  MarkContextChanged(kCanvasChanged);
}
