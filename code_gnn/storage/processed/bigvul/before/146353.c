void WebGLRenderingContextBase::ResetUnpackParameters() {
  if (unpack_alignment_ != 1)
    ContextGL()->PixelStorei(GL_UNPACK_ALIGNMENT, 1);
}
