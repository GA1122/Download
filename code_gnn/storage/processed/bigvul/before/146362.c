void WebGLRenderingContextBase::RestoreUnpackParameters() {
  if (unpack_alignment_ != 1)
    ContextGL()->PixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment_);
}
