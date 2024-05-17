  explicit FenceCallback()
      : fence(gfx::GLFence::Create()) {
    DCHECK(fence);
  }
