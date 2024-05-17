void GpuChannel::DidDestroyCommandBuffer(gfx::GpuPreference gpu_preference) {
  if (gpu_preference == gfx::PreferDiscreteGpu)
    --num_contexts_preferring_discrete_gpu_;
  DCHECK_GE(num_contexts_preferring_discrete_gpu_, 0);
}