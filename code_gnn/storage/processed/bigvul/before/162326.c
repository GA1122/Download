MojoVideoEncodeAccelerator::GetSupportedProfiles() {
  DVLOG(1) << __func__;
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  return GpuVideoAcceleratorUtil::ConvertGpuToMediaEncodeProfiles(
      supported_profiles_);
}
