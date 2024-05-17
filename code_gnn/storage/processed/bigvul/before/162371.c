MojoVideoEncodeAcceleratorService::MojoVideoEncodeAcceleratorService(
    const CreateAndInitializeVideoEncodeAcceleratorCallback&
        create_vea_callback,
    const gpu::GpuPreferences& gpu_preferences)
    : create_vea_callback_(create_vea_callback),
      gpu_preferences_(gpu_preferences),
      output_buffer_size_(0),
      weak_factory_(this) {
  DVLOG(1) << __func__;
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
}
