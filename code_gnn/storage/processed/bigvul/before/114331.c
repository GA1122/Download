void WebGraphicsContext3DCommandBufferImpl::prepareTexture() {
  TRACE_EVENT1("gpu",
                "WebGraphicsContext3DCommandBufferImpl::SwapBuffers",
                "frame", frame_number_);
  frame_number_++;
  if (ShouldUseSwapClient())
    swap_client_->OnViewContextSwapBuffersPosted();

  if (command_buffer_->GetLastState().error == gpu::error::kNoError)
    gl_->SwapBuffers();

  command_buffer_->Echo(base::Bind(
      &WebGraphicsContext3DCommandBufferImpl::OnSwapBuffersComplete,
      weak_ptr_factory_.GetWeakPtr()));
#if defined(OS_MACOSX)
  gl_->Flush();
#endif
}
