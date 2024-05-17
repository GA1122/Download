void WebGL2RenderingContextBase::drawElementsInstanced(GLenum mode,
                                                       GLsizei count,
                                                       GLenum type,
                                                       long long offset,
                                                       GLsizei instance_count) {
  if (!ValidateDrawElements("drawElementsInstanced", type, offset))
    return;

  if (!bound_vertex_array_object_->IsAllEnabledAttribBufferBound()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "drawElementsInstanced",
                      "no buffer is bound to enabled attribute");
    return;
  }

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.Get());
  ClearIfComposited();
  ContextGL()->DrawElementsInstancedANGLE(
      mode, count, type, reinterpret_cast<void*>(static_cast<intptr_t>(offset)),
      instance_count);
  MarkContextChanged(kCanvasChanged);
}
