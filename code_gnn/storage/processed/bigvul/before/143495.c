void CompositorImpl::DidSwapBuffers(const gfx::Size& swap_size) {
  client_->DidSwapBuffers(swap_size);

  if (swap_completed_with_size_for_testing_)
    swap_completed_with_size_for_testing_.Run(swap_size);
}
