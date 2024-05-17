error::Error GLES2DecoderPassthroughImpl::DoWaitGpuFenceCHROMIUM(
    GLuint gpu_fence_id) {
  if (!feature_info_->feature_flags().chromium_gpu_fence)
    return error::kUnknownCommand;
  if (!GetGpuFenceManager()->GpuFenceServerWait(gpu_fence_id))
    return error::kInvalidArguments;
  return error::kNoError;
}
