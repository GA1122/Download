WebGraphicsContext3DCommandBufferImpl::WebGraphicsContext3DCommandBufferImpl(
    int surface_id,
    const GURL& active_url,
    const base::WeakPtr<WebGraphicsContext3DSwapBuffersClient>& swap_client)
    : initialize_failed_(false),
      context_(NULL),
      gl_(NULL),
      host_(NULL),
      surface_id_(surface_id),
      active_url_(active_url),
      swap_client_(swap_client),
      memory_allocation_changed_callback_(0),
      context_lost_callback_(0),
      context_lost_reason_(GL_NO_ERROR),
      error_message_callback_(0),
      swapbuffers_complete_callback_(0),
      gpu_preference_(gfx::PreferIntegratedGpu),
      cached_width_(0),
      cached_height_(0),
      bound_fbo_(0),
      weak_ptr_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {
}
