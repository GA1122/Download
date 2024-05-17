GpuFence* GpuFence::FromClientGpuFence(ClientGpuFence gpu_fence) {
   return reinterpret_cast<GpuFence*>(gpu_fence);
 }
