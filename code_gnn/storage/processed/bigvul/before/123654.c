GpuMemoryManager* GpuCommandBufferStub::GetMemoryManager() {
    return channel()->gpu_channel_manager()->gpu_memory_manager();
}
