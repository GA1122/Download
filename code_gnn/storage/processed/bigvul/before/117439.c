void WebGraphicsContext3DCommandBufferImpl::OnContextLost(
    ContentGLContext::ContextLostReason reason) {
  context_lost_reason_ = convertReason(reason);
  if (context_lost_callback_) {
    context_lost_callback_->onContextLost();
  }
  if (attributes_.shareResources)
    ClearSharedContexts();
  if (ShouldUseSwapClient())
    swap_client_->OnViewContextSwapBuffersAborted();
}
