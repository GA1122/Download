void WebGL2RenderingContextBase::drawArraysInstanced(GLenum mode,
                                                     GLint first,
                                                     GLsizei count,
                                                     GLsizei instance_count) {
  if (!ValidateDrawArrays("drawArraysInstanced"))
    return;

  if (!bound_vertex_array_object_->IsAllEnabledAttribBufferBound()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "drawArraysInstanced",
                      "no buffer is bound to enabled attribute");
    return;
  }

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.Get());
  ClearIfComposited();
  ContextGL()->DrawArraysInstancedANGLE(mode, first, count, instance_count);
  MarkContextChanged(kCanvasChanged);
}
