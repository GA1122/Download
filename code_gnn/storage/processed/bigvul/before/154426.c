error::Error GLES2DecoderPassthroughImpl::CheckSwapBuffersResult(
    gfx::SwapResult result,
    const char* function_name) {
  if (result == gfx::SwapResult::SWAP_FAILED) {
    LOG(ERROR) << "Context lost because " << function_name << " failed.";
    if (!CheckResetStatus()) {
      MarkContextLost(error::kUnknown);
      group_->LoseContexts(error::kUnknown);
      return error::kLostContext;
    }
  }

  return error::kNoError;
}
