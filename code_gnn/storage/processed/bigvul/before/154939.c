    DrawingBufferClientRestoreRenderbufferBinding() {
  if (destruction_in_progress_)
    return;
  if (!ContextGL())
    return;
  ContextGL()->BindRenderbuffer(GL_RENDERBUFFER,
                                ObjectOrZero(renderbuffer_binding_.Get()));
}
