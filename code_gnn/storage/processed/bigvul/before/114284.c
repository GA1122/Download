void WebGraphicsContext3DCommandBufferImpl::OnSwapBuffersComplete() {
  typedef WebGraphicsContext3DSwapBuffersClient WGC3DSwapClient;
  if (ShouldUseSwapClient()) {
    MessageLoop::current()->PostTask(FROM_HERE, base::Bind(
        &WGC3DSwapClient::OnViewContextSwapBuffersComplete, swap_client_));
  }

  if (swapbuffers_complete_callback_)
    swapbuffers_complete_callback_->onSwapBuffersComplete();
}
