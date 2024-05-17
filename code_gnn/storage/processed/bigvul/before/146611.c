void DrawingBuffer::CleanupRGBEmulationForBlitFramebuffer() {
  DCHECK(back_color_buffer_);
  DCHECK(back_color_buffer_->image_id);
  GLenum target = GC3D_TEXTURE_RECTANGLE_ARB;
  gl_->FramebufferTexture2D(GL_DRAW_FRAMEBUFFER_ANGLE, GL_COLOR_ATTACHMENT0,
                            target, back_color_buffer_->texture_id, 0);
  gl_->ColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
  gl_->Disable(GL_SCISSOR_TEST);
  gl_->ClearColor(0, 0, 0, 1);
  gl_->Clear(GL_COLOR_BUFFER_BIT);
  DCHECK(client_);
  client_->DrawingBufferClientRestoreScissorTest();
  client_->DrawingBufferClientRestoreMaskAndClearValues();
}
