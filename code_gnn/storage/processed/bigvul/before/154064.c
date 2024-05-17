void GLES2DecoderImpl::DoScissor(GLint x,
                                 GLint y,
                                 GLsizei width,
                                 GLsizei height) {
  gfx::Vector2d draw_offset = GetBoundFramebufferDrawOffset();
  api()->glScissorFn(x + draw_offset.x(), y + draw_offset.y(), width, height);
}
