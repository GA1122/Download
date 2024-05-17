    DrawingBufferClientRestorePixelPackParameters() {
  if (destruction_in_progress_)
    return;
  if (!ContextGL())
    return;
  ContextGL()->PixelStorei(GL_PACK_ALIGNMENT, pack_alignment_);
}
