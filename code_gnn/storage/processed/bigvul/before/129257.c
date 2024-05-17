error::Error GLES2DecoderImpl::DoClear(GLbitfield mask) {
  DCHECK(!ShouldDeferDraws());
  if (CheckBoundFramebuffersValid("glClear")) {
    ApplyDirtyState();
    glClear(mask);
  }
  return error::kNoError;
}
