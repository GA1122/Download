bool GpuChannel::ShouldPreferDiscreteGpu() const {
   return num_contexts_preferring_discrete_gpu_ > 0;
 }
