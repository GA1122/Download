void MojoVideoEncodeAcceleratorService::Create(
    mojom::VideoEncodeAcceleratorRequest request,
    const CreateAndInitializeVideoEncodeAcceleratorCallback&
        create_vea_callback,
    const gpu::GpuPreferences& gpu_preferences) {
  mojo::MakeStrongBinding(std::make_unique<MojoVideoEncodeAcceleratorService>(
                              create_vea_callback, gpu_preferences),
                          std::move(request));
}
