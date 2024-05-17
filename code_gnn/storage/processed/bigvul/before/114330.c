void WebGraphicsContext3DCommandBufferImpl::postSubBufferCHROMIUM(
    int x, int y, int width, int height) {
  if (ShouldUseSwapClient())
    swap_client_->OnViewContextSwapBuffersPosted();
  gl_->PostSubBufferCHROMIUM(x, y, width, height);
  command_buffer_->Echo(base::Bind(
      &WebGraphicsContext3DCommandBufferImpl::OnSwapBuffersComplete,
      weak_ptr_factory_.GetWeakPtr()));
}
