void GpuChannel::WillCreateCommandBuffer(gfx::GpuPreference gpu_preference) {
  if (gpu_preference == gfx::PreferDiscreteGpu)
    ++num_contexts_preferring_discrete_gpu_;
}
