gfx::SwapResult GLSurfaceOzoneEGL::SwapBuffers() {
  gfx::SwapResult result = NativeViewGLSurfaceEGL::SwapBuffers();
  if (result != gfx::SwapResult::SWAP_ACK)
    return result;

  return ozone_surface_->OnSwapBuffers() ? gfx::SwapResult::SWAP_ACK
                                         : gfx::SwapResult::SWAP_FAILED;
}
