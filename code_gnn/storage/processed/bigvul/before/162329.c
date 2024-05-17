MojoVideoEncodeAccelerator::MojoVideoEncodeAccelerator(
    mojom::VideoEncodeAcceleratorPtr vea,
    const gpu::VideoEncodeAcceleratorSupportedProfiles& supported_profiles)
    : vea_(std::move(vea)), supported_profiles_(supported_profiles) {
  DVLOG(1) << __func__;
  DCHECK(vea_);
}
