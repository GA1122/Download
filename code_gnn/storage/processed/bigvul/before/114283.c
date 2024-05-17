void WebGraphicsContext3DCommandBufferImpl::OnMemoryAllocationChanged(
    const GpuMemoryAllocationForRenderer& allocation) {
  if (memory_allocation_changed_callback_)
    memory_allocation_changed_callback_->onMemoryAllocationChanged(
        WebKit::WebGraphicsMemoryAllocation(
            allocation.gpu_resource_size_in_bytes,
            allocation.suggest_have_backbuffer));
}
