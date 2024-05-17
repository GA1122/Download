void DrawingBuffer::ResolveIfNeeded() {
  if (anti_aliasing_mode_ != kNone && !contents_change_resolved_)
    ResolveMultisampleFramebufferInternal();
  contents_change_resolved_ = true;
}
