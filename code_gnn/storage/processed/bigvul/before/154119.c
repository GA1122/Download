void GLES2DecoderImpl::FinishSwapBuffers(gfx::SwapResult result) {
  if (result == gfx::SwapResult::SWAP_FAILED) {
    LOG(ERROR) << "Context lost because SwapBuffers failed.";
    if (!CheckResetStatus()) {
      MarkContextLost(error::kUnknown);
      group_->LoseContexts(error::kUnknown);
    }
  }
  ++swaps_since_resize_;
  if (swaps_since_resize_ == 1 && surface_->BuffersFlipped()) {
    backbuffer_needs_clear_bits_ |= GL_COLOR_BUFFER_BIT;
  }
}
