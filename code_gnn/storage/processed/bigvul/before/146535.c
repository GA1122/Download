void WebGLRenderingContextBase::scissor(GLint x,
                                        GLint y,
                                        GLsizei width,
                                        GLsizei height) {
  if (isContextLost())
    return;
  scissor_box_[0] = x;
  scissor_box_[1] = y;
  scissor_box_[2] = width;
  scissor_box_[3] = height;
  ContextGL()->Scissor(x, y, width, height);
}
