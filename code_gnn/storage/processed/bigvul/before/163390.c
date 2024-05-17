void RenderThreadImpl::ReleaseFreeMemory() {
  base::allocator::ReleaseFreeMemory();
  discardable_shared_memory_manager_->ReleaseFreeMemory();

  if (blink_platform_impl_)
    blink::DecommitFreeableMemory();
}
