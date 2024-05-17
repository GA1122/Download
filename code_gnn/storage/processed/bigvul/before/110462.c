error::Error GLES2DecoderImpl::DoClear(GLbitfield mask) {
  if (ShouldDeferDraws())
    return error::kDeferCommandUntilLater;
  if (CheckBoundFramebuffersValid("glClear")) {
    UNSHIPPED_TRACE_EVENT_INSTANT2("test_gpu", "DoClear", "red", clear_red_,
                                   "green", clear_green_);
    ApplyDirtyState();
    glClear(mask);
  }
  return error::kNoError;
}
