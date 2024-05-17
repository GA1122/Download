void WebGraphicsContext3DCommandBufferImpl::OnContextLost() {
  context_lost_reason_ = convertReason(
      command_buffer_->GetLastState().context_lost_reason);
  if (context_lost_callback_) {
    context_lost_callback_->onContextLost();
  }
  if (attributes_.shareResources)
    ClearSharedContexts();
  if (ShouldUseSwapClient())
    swap_client_->OnViewContextSwapBuffersAborted();
}
